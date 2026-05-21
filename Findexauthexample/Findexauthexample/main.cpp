#include "findexauth.hpp"
#include <conio.h>
#include <iostream>
#include <string>

const std::string APP_NAME = "Findex Internal";
const std::string OWNER_ID = "Your-Owner-ID";
const std::string APP_SECRET = "5715b39369d3a6a763992813c037bf335a368f94f9d82287be12389f4d0ba4de";
const std::string APP_VER = "1.0";
const std::string SERVER_URL = "https://findexauth.online";

void setColor(int c) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); }
void magenta() { setColor(13); }
void cyan() { setColor(11); }
void green() { setColor(10); }
void red() { setColor(12); }
void yellow() { setColor(14); }
void darkGray() { setColor(8); }
void white() { setColor(15); }
void reset() { setColor(7); }

std::string format_expiry(const std::string& exp) {
    if (exp.empty() || exp == "Lifetime") return "Lifetime";
    std::string out = exp;
    size_t t = out.find('T');
    if (t != std::string::npos) out[t] = ' ';
    size_t z = out.find('.');
    if (z != std::string::npos) out = out.substr(0, z);
    else {
        z = out.find('Z');
        if (z != std::string::npos) out = out.substr(0, z);
    }
    return out;
}

void draw_banner() {
    system("cls");
    magenta();
    std::cout << "\n";
    std::cout << "  \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF\n";
    std::cout << "  \xB3                                                            \xB3\n";
    std::cout << "  \xB3  \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB2\xDB\xDB\xB2\xDB\xDB\xDB\xB2   \xDB\xDB\xB2\xDB\xDB\xDB\xDB\xDB\xDB\xB2 \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB2\xDB\xDB\xB2  \xDB\xDB\xB2               \xB3\n";
    std::cout << "  \xB3  \xDB\xDB\xB2\xDF\xDF\xDF\xDF\xB2\xDB\xDB\xB2\xDB\xDB\xDB\xDB\xB2  \xDB\xDB\xB2\xDB\xDB\xB2\xDF\xDF\xDB\xDB\xB2\xDB\xDB\xB2\xDF\xDF\xDF\xDF\xB2\xDF\xDB\xDB\xB2\xDB\xDB\xDF                \xB3\n";
    std::cout << "  \xB3  \xDB\xDB\xDB\xDB\xDB\xB2  \xDB\xDB\xB2\xDB\xDB\xB2\xDF\xDB\xDB\xB2 \xDB\xDB\xB2\xDB\xDB\xB2  \xDB\xDB\xB2\xDB\xDB\xDB\xDB\xDB\xB2   \xDF\xDB\xDB\xDB\xDF                 \xB3\n";
    std::cout << "  \xB3  \xDB\xDB\xB2\xDF\xDF\xB2  \xDB\xDB\xB2\xDB\xDB\xB2 \xDF\xDB\xDB\xB2\xDB\xDB\xB2\xDB\xDB\xB2  \xDB\xDB\xB2\xDB\xDB\xB2\xDF\xDF\xB2   \xDB\xDB\xB2\xDB\xDB\xB2                \xB3\n";
    std::cout << "  \xB3  \xDB\xDB\xB2     \xDB\xDB\xB2\xDB\xDB\xB2  \xDF\xDB\xDB\xDB\xB2\xDB\xDB\xDB\xDB\xDB\xDB\xB2\xDF\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB2\xDB\xDB\xB2\xDF \xDB\xDB\xB2               \xB3\n";
    std::cout << "  \xB3  \xDF\xDF\xB2     \xDF\xDF\xB2\xDF\xDF\xB2   \xDF\xDF\xDF\xB2\xDF\xDF\xDF\xDF\xDF\xB2 \xDF\xDF\xDF\xDF\xDF\xDF\xB2\xDF\xDF\xB2  \xDF\xDF\xB2               \xB3\n";
    std::cout << "  \xB3                                                            \xB3\n";
    std::cout << "  \xB3           \xDB\xDB\xDB\xDB\xDB\xB2 \xDB\xDB\xB2   \xDB\xDB\xB2\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB2\xDB\xDB\xB2  \xDB\xDB\xB2                  \xB3\n";
    std::cout << "  \xB3          \xDB\xDB\xB2\xDF\xDF\xDB\xDB\xB2\xDB\xDB\xB2   \xDB\xDB\xB2\xDF\xDF\xDB\xDB\xB2\xDF\xDF\xB2\xDB\xDB\xB2  \xDB\xDB\xB2                  \xB3\n";
    std::cout << "  \xB3          \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB2\xDB\xDB\xB2   \xDB\xDB\xB2   \xDB\xDB\xB2   \xDB\xDB\xDB\xDB\xDB\xDB\xDB\xB2                  \xB3\n";
    std::cout << "  \xB3          \xDB\xDB\xB2\xDF\xDF\xDB\xDB\xB2\xDB\xDB\xB2   \xDB\xDB\xB2   \xDB\xDB\xB2   \xDB\xDB\xB2\xDF\xDF\xDB\xDB\xB2                  \xB3\n";
    std::cout << "  \xB3          \xDB\xDB\xB2  \xDB\xDB\xB2\xDF\xDB\xDB\xDB\xDB\xDB\xDB\xB2\xDF   \xDB\xDB\xB2   \xDB\xDB\xB2  \xDB\xDB\xB2                  \xB3\n";
    std::cout << "  \xB3          \xDF\xDF\xB2  \xDF\xDF\xB2 \xDF\xDF\xDF\xDF\xDF\xB2    \xDF\xDF\xB2   \xDF\xDF\xB2  \xDF\xDF\xB2                  \xB3\n";
    std::cout << "  \xB3                                                            \xB3\n";
    std::cout << "  \xB3           License Verification System v1.0.0               \xB3\n";
    std::cout << "  \xB3                                                            \xB3\n";
    std::cout << "  \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9\n\n";
    reset();
}

std::string prompt(const std::string& label, bool hidden = false) {
    white(); std::cout << "  " << label << " > "; yellow();
    if (!hidden) { std::string s; std::getline(std::cin, s); reset(); return s; }
    std::string s; char c;
    while ((c = _getch()) != '\r') {
        if (c == '\b' && !s.empty()) { std::cout << "\b \b"; s.pop_back(); }
        else if (c != '\b') { std::cout << '*'; s += c; }
    }
    std::cout << '\n'; reset(); return s;
}

void run_protected_app(FindexAuth::api& auth) {
    system("cls");
    green();
    std::cout << "\n  \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF\n";
    std::cout << "  \xB3                                                            \xB3\n";
    std::cout << "  \xB3                    " << char(251) << " ACCESS GRANTED " << char(251) << "                        \xB3\n";
    std::cout << "  \xB3                                                            \xB3\n";
    std::cout << "  \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9\n\n";
    reset();

    cyan(); std::cout << "  Welcome to the protected application, " << auth.user_data.username << "!\n\n"; reset();
    std::cout << "  " << std::string(60, '-') << "\n";
    white();
    std::cout << "  This is your protected application area.\n";
    std::cout << "  Only users with valid licenses can access this section.\n";
    reset();
    std::cout << "  " << std::string(60, '-') << "\n\n";

    auth.start_heartbeat(30);

    while (true) {
        yellow(); std::cout << "  Available Commands:\n"; white();
        std::cout << "    1. Show User Info\n";
        std::cout << "    2. Check License Status\n";
        std::cout << "    3. View Application Info\n";
        std::cout << "    4. Logout\n\n";
        
        cyan(); std::cout << "  Enter command (1-4) > "; yellow();
        std::string choice; std::getline(std::cin, choice); reset();

        if (choice == "1") {
            cyan(); std::cout << "\n  [User Info]\n"; reset();
            std::cout << "    Username: " << auth.user_data.username << "\n";
            std::cout << "    License Key: " << auth.user_data.license_key << "\n";
            std::cout << "    Expires: " << format_expiry(auth.user_data.expiry) << "\n";
            green(); std::cout << "    Status: Active\n";
            std::cout << "    Access Level: " << auth.user_data.level << "\n\n"; reset();
        } else if (choice == "2") {
            cyan(); std::cout << "\n  [License Status]\n"; reset();
            std::cout << "    Expires: " << format_expiry(auth.user_data.expiry) << "\n";
            green(); std::cout << "    " << char(251) << " License: Valid\n";
            std::cout << "    " << char(251) << " Hardware Binding: Active\n";
            std::cout << "    " << char(251) << " Connection: Secure\n\n"; reset();
        } else if (choice == "3") {
            cyan(); std::cout << "\n  [Application Info]\n"; reset();
            std::cout << "    App Name: " << APP_NAME << "\n";
            std::cout << "    Version: " << APP_VER << "\n";
            std::cout << "    Protected By: FindexAuth\n\n";
        } else if (choice == "4") {
            yellow(); std::cout << "\n  [*] Logging out...\n\n"; reset();
            ExitProcess(0);
        } else {
            red(); std::cout << "\n  [ERROR] Invalid command. Please enter 1-4.\n\n"; reset();
        }
    }
}

int main() {
    SetConsoleTitleA("FindexAuth - License Verification System");
    draw_banner();
    
    cyan(); std::cout << "  [INFO] Initializing FindexAuth Client...\n"; reset();
    FindexAuth::api auth(APP_NAME, OWNER_ID, APP_SECRET, APP_VER, SERVER_URL);

    // Dummy HWID for display (real one used internally)
    darkGray(); std::cout << "\n  [HWID] Your Hardware ID: " << auth.user_data.hwid.substr(0, 16) << "...\n"; reset();

    yellow(); std::cout << "\n  [*] Connecting to FindexAuth Server...\n"; reset();
    auth.init();
    if (!auth.response.success) {
        red(); std::cout << "\n  [ERROR] Failed to connect: " << auth.response.message << "\n"; reset();
        std::cout << "\n  Press any key to exit..."; _getch(); return 1;
    }
    
    green(); std::cout << "  [SUCCESS] Connected to FindexAuth!\n";
    std::cout << "  [INFO] App: " << APP_NAME << "\n"; reset();
    green(); std::cout << "  [INFO] Version: " << APP_VER << " " << char(251) << "\n\n"; reset();

    while (true) {
        white(); std::cout << "  " << std::string(60, '=') << "\n";
        std::cout << "  Choose an option:\n";
        std::cout << "    1. Login with Username & Password\n";
        std::cout << "    2. Register New Account (with License Key)\n";
        std::cout << "    3. Exit\n\n";
        
        cyan(); std::cout << "  > "; yellow();
        std::string choice; std::getline(std::cin, choice); reset();

        if (choice == "1") {
            cyan(); std::cout << "\n  \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF\n";
            std::cout << "  \xB3                        LOGIN                               \xB3\n";
            std::cout << "  \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9\n\n"; reset();
            
            std::string uname = prompt("Username");
            if (uname.empty()) { red(); std::cout << "  [ERROR] Username cannot be empty!\n"; reset(); continue; }
            
            std::string pass = prompt("Password", true);
            if (pass.empty()) { red(); std::cout << "  [ERROR] Password cannot be empty!\n"; reset(); continue; }
            
            yellow(); std::cout << "\n  [*] Logging in...\n"; reset();
            auth.login(uname, pass);
            
            if (auth.response.success) {
                green(); std::cout << "\n  \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF\n";
                std::cout << "  \xB3                  " << char(251) << " LOGIN SUCCESSFUL " << char(251) << "                      \xB3\n";
                std::cout << "  \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9\n\n"; reset();
                
                cyan(); std::cout << "  Welcome, " << auth.user_data.username << "!\n"; reset();
                std::cout << "  License Key: " << auth.user_data.license_key << "\n";
                std::cout << "  Expires: " << format_expiry(auth.user_data.expiry) << "\n\n";
                
                std::cout << "  Press any key to continue..."; _getch();
                run_protected_app(auth);
                break;
            } else {
                red(); std::cout << "\n  " << char(253) << " LOGIN FAILED: " << auth.response.message << "\n"; reset();
            }
        }
        else if (choice == "2") {
            magenta(); std::cout << "\n  \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF\n";
            std::cout << "  \xB3                  REGISTER NEW ACCOUNT                      \xB3\n";
            std::cout << "  \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9\n\n"; reset();
            
            std::string key = prompt("License Key");
            if (key.empty()) { red(); std::cout << "  [ERROR] License key cannot be empty!\n"; reset(); continue; }
            
            std::string uname = prompt("Username");
            if (uname.empty()) { red(); std::cout << "  [ERROR] Username cannot be empty!\n"; reset(); continue; }
            
            std::string pass = prompt("Password (min 6 chars)", true);
            std::string confirm = prompt("Confirm Password", true);
            
            if (pass != confirm) {
                red(); std::cout << "\n  [ERROR] Passwords do not match!\n"; reset(); continue;
            }
            
            yellow(); std::cout << "\n  [*] Verifying license and registering account...\n"; reset();
            
            auth.license(key);
            if (auth.response.success && auth.response.needs_registration) {
                auth.register_key(auth.response.validated_key, uname, pass);
                if (auth.response.success) {
                    green(); std::cout << "\n  \xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF\n";
                    std::cout << "  \xB3            " << char(251) << " REGISTRATION SUCCESSFUL " << char(251) << "                     \xB3\n";
                    std::cout << "  \xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9\n\n"; reset();
                    
                    cyan(); std::cout << "  Username: " << auth.user_data.username << "\n"; reset();
                    std::cout << "  License Key: " << auth.user_data.license_key << "\n";
                    std::cout << "  Expires: " << format_expiry(auth.user_data.expiry) << "\n\n";
                    
                    green(); std::cout << "  " << char(251) << " You can now login with your credentials!\n"; reset();
                } else {
                    red(); std::cout << "\n  " << char(253) << " REGISTRATION FAILED: " << auth.response.message << "\n"; reset();
                }
            } else {
                std::string msg = !auth.response.success ? auth.response.message : "License key is already registered to an account.";
                red(); std::cout << "\n  " << char(253) << " REGISTRATION FAILED: " << msg << "\n"; reset();
            }
        }
        else if (choice == "3") {
            cyan(); std::cout << "\n  Goodbye!\n"; reset();
            break;
        } else {
            red(); std::cout << "\n  [ERROR] Invalid choice. Please enter 1-3.\n"; reset();
        }
    }
    
    return 0;
}