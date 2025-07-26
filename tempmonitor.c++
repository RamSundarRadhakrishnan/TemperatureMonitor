#include <gtkmm/main.h>
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <memory>
#include <array>

std::string execute_command(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

void display_alert(const std::string& message) {
    auto app = Gtk::Application::create();
    Gtk::Window alert_window;
    alert_window.set_title("High Temperature Alert");
    alert_window.set_default_size(300, 100);
    Gtk::Box box(Gtk::ORIENTATION_VERTICAL, 10);
    alert_window.add(box);

    Gtk::Label label;
    label.set_markup("<span font='12' weight='bold' color='red'>" + message + "</span>");
    box.pack_start(label, Gtk::PACK_SHRINK);

    alert_window.show_all_children();
    app->run(alert_window);
}

class TemperatureWindow : public Gtk::Window {
public:
    TemperatureWindow() {
        set_title("System Sensor Readings");
        set_default_size(400, 300);

        add(vbox);

        // Title Label
        auto title_label = Gtk::manage(new Gtk::Label());
        title_label->set_markup("<span font='16' weight='bold' color='black'>Temperature Sensor Readings</span>");
        vbox.pack_start(*title_label, Gtk::PACK_SHRINK);

        // CPU Temperature
        auto cpu_label = Gtk::manage(new Gtk::Label());
        std::string cpu_temp = execute_command("sensors | grep -E 'Core|Package'");
        std::stringstream cpu_markup;
        cpu_markup << "<span font='12' color='blue'>CPU Temperatures:</span>\n" << cpu_temp;
        cpu_label->set_markup(cpu_markup.str());
        vbox.pack_start(*cpu_label, Gtk::PACK_SHRINK);

        std::string package_temp_line = execute_command("sensors | grep -m 1 'Package'");
        size_t start = package_temp_line.find("+");
        size_t end = package_temp_line.find("C", start);
        int package_temp = (start != std::string::npos && end != std::string::npos) ? std::stoi(package_temp_line.substr(start+1, end-start-1)) : 0;

        // GPU Temperature
        auto gpu_label = Gtk::manage(new Gtk::Label());
        std::string gpu_temp = execute_command("nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader").substr(0,2);
        std::stringstream gpu_markup;
        gpu_markup << "<span font='12' color='green'>GPU Temperature:</span> " << gpu_temp << "°C";
        gpu_label->set_markup(gpu_markup.str());
        vbox.pack_start(*gpu_label, Gtk::PACK_SHRINK);

        // Battery info
        auto battery_label = Gtk::manage(new Gtk::Label());
        std::string battery_status = execute_command("acpi -b");
        std::stringstream battery_markup;
        battery_markup << "<span font='12' color='purple'>Battery Status:</span>\n" << battery_status;
        battery_label->set_markup(battery_markup.str());
        vbox.pack_start(*battery_label, Gtk::PACK_SHRINK);

        bool cpu_warning = package_temp > 100;
        bool gpu_warning = !gpu_temp.empty() && std::stoi(gpu_temp) > 75;

        if (cpu_warning) {
            display_alert("Warning: CPU Temperature Exceeds 100°C!");
        }
        if (gpu_warning) {
            display_alert("Warning: GPU Temperature Exceeds 75°C!");
        }

        show_all_children();
    }

private:
    Gtk::Box vbox {Gtk::ORIENTATION_VERTICAL, 10};
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
    TemperatureWindow window;
    return app->run(window);
}
