#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

// 🎨 ANSI color codes
#define RESET     "\033[0m"
#define GREEN     "\033[32m"
#define YELLOW    "\033[33m"
#define ORANGE    "\033[38;5;208m"
#define CYAN      "\033[36m"
#define RED       "\033[31m"
#define BOLD      "\033[1m"
#define UNDERLINE "\033[4m"

struct WaterSource {
    std::string name;
    int capacity;
    int initial_capacity;
    std::string area;
    int population;
};

enum Season { SPRING, SUMMER, AUTUMN, WINTER };

// 🌧️ Rainfall logic per season
int getRainfall(Season season) {
    switch (season) {
        case SPRING: return rand() % 41 + 20;   // 20–60
        case SUMMER: return rand() % 31;        // 0–30
        case AUTUMN: return rand() % 31 + 10;   // 10–40
        case WINTER: return rand() % 16 + 5;    // 5–20
        default: return 0;
    }
}

// 🎨 Season color mapping
std::string getSeasonColor(Season season) {
    switch (season) {
        case SPRING: return GREEN;
        case SUMMER: return YELLOW;
        case AUTUMN: return ORANGE;
        case WINTER: return CYAN;
        default: return RESET;
    }
}

// 🌸 ASCII art per season
void showSeasonArt(Season season) {
    switch (season) {
        case SPRING:
            std::cout << GREEN << "🌸🌿 Spring blossoms and fresh rain!\n" << RESET;
            break;
        case SUMMER:
            std::cout << YELLOW << "☀️🔥 Summer heat and dry spells!\n" << RESET;
            break;
        case AUTUMN:
            std::cout << ORANGE << "🍁🌾 Autumn winds and golden leaves!\n" << RESET;
            break;
        case WINTER:
            std::cout << CYAN << "❄️🌨️ Winter chill and snowflakes!\n" << RESET;
            break;
    }
}

// 🔋 Animated tank bar
void animateTankBar(int barLength, std::string color) {
    std::cout << "🔋 Tank Level: " << color << "[";
    for (int i = 0; i < 20; ++i) {
        std::cout << (i < barLength ? "#" : "-");
        std::cout.flush();
        
    }
    std::cout << "]" << RESET << "\n";
}

// 📅 Simulate a day
void simulateDay(std::vector<WaterSource>& sources, Season season) {
    std::cout << "\n📅 Simulating a new day...\n";
    std::string color = getSeasonColor(season);
    showSeasonArt(season);

    for (auto& source : sources) {
        int rainfall = getRainfall(season);
        source.capacity += rainfall;
        std::cout << "🌧️ " << source.name << " received " << rainfall << "L of rainfall.\n";

        int usage = source.population * 2;
        source.capacity -= usage;
        if (source.capacity < 0) source.capacity = 0;

        std::cout << "🚰 " << usage << "L used in " << source.area << ". Remaining: " << source.capacity << "L\n";

        double level = (double)source.capacity / source.initial_capacity;
        if (level < 0.1) {
            std::cout << RED << BOLD << "🚨 ALERT: " << source.name << " is below 10% capacity!" << RESET << "\n";
        }

        int barLength = (source.capacity * 20) / source.initial_capacity;
        animateTankBar(barLength, color);
    }
}

// ✏️ Update water source
void updateWaterSource(std::vector<WaterSource>& sources) {
    std::string name;
    std::cout << "\n✏️ Enter water source name to update: ";
    std::cin >> name;

    bool found = false;
    for (auto& source : sources) {
        if (source.name == name) {
            std::cout << "Enter new capacity (L): ";
            std::cin >> source.capacity;
            std::cout << "✅ " << source.name << " updated to " << source.capacity << "L\n";
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "⚠️ Source not found.\n";
    }
}

// 💾 Save data to file
void saveData(const std::vector<WaterSource>& sources) {
    std::ofstream file("water_data.txt");
    if (!file) {
        std::cout << "❌ Failed to save data.\n";
        return;
    }

    file << "💧 Final Water Source Status:\n";
    for (const auto& source : sources) {
        file << "Name: " << source.name << "\n";
        file << "Area: " << source.area << "\n";
        file << "Population: " << source.population << "\n";
        file << "Capacity: " << source.capacity << "L\n";
        file << "Initial Capacity: " << source.initial_capacity << "L\n";
        file << "--------------------------\n";
    }

    file.close();
    std::cout << "💾 Data saved to water_data.txt\n";
}

// 📊 Show graphical summary
void showSummary(const std::vector<WaterSource>& sources) {
    std::cout << BOLD << "\n📊 Final Status\n" << RESET;
    for (const auto& source : sources) {
        double level = (double)source.capacity / source.initial_capacity;
        int barLength = level * 20;
        std::string color = (level > 0.6) ? GREEN : (level > 0.3) ? YELLOW : RED;

        std::cout << "💧 " << source.name << " (" << source.area << ") - " << source.capacity << "L\n";
        std::cout << "🔋 " << color << "[";
        for (int i = 0; i < 20; ++i) {
            std::cout << (i < barLength ? "#" : "-");
        }
        std::cout << "]" << RESET << "\n";
    }
}

// 🌤️ Choose season
Season chooseSeason() {
    int seasonChoice;
    std::cout << "\n🌤️ Choose season:\n";
    std::cout << "0 - Spring\n1 - Summer\n2 - Autumn\n3 - Winter\nYour choice: ";
    std::cin >> seasonChoice;
    return static_cast<Season>(seasonChoice);
}

// 📋 Main menu
int showMainMenu() {
    std::cout << BOLD << "\n📋 Main Menu\n" << RESET;
    std::cout << "1️⃣ Simulate a day\n";
    std::cout << "2️⃣ Update a water source\n";
    std::cout << "3️⃣ Change season\n";
    std::cout << "4️⃣ View status\n";
    std::cout << "5️⃣ Exit\n";
    std::cout << "Choose an option: ";
    int choice;
    std::cin >> choice;
    return choice;
}

// 🚀 Main function
int main() {
    srand(time(0));

    std::vector<WaterSource> sources;
    int numSources;

    std::cout << BOLD << "💧 Smart Water Distribution System\n" << RESET;
    std::cout << "Enter number of water sources: ";
    std::cin >> numSources;

    for (int i = 0; i < numSources; ++i) {
        WaterSource ws;
        std::cout << "\n🔹 Enter details for source " << i + 1 << ":\n";
        std::cout << "Name: ";
        std::cin >> ws.name;
        std::cout << "Capacity (L): ";
        std::cin >> ws.capacity;
        ws.initial_capacity = ws.capacity;
        std::cout << "Area: ";
        std::cin >> ws.area;
        std::cout << "Population: ";
        std::cin >> ws.population;

        sources.push_back(ws);
    }

    Season season = chooseSeason();
    bool running = true;

    while (running) {
        int choice = showMainMenu();
        switch (choice) {
            case 1:
                simulateDay(sources, season);
                break;
            case 2:
                updateWaterSource(sources);
                break;
            case 3:
                season = chooseSeason();
                break;
            case 4:
                showSummary(sources);
                break;
            case 5:
                running = false;
                break;
            default:
                std::cout << "❌ Invalid choice.\n";
        }
    }

    saveData(sources);
    std::cout << "\n✅ Simulation complete.\n";
    return 0;
}