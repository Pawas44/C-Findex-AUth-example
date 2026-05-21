# FindexAuth - C++ Example

Welcome to the **FindexAuth C++ SDK Example**. This project demonstrates how to integrate the FindexAuth authentication and licensing platform into any native C++ application (such as game cheats, trainers, or native tools) using an elegant Console UI similar to CloudAuth.

FindexAuth provides military-grade security, hardware binding (HWID), and a robust session management system using native Windows APIs, without requiring bulky third-party libraries.

## Features
- **No External Dependencies:** The `findexauth.hpp` header relies purely on native Windows APIs (`WinHTTP`, `Advapi32`). No need to install `libcurl`, `nlohmann/json`, or OpenSSL.
- **Plug & Play:** Drop `findexauth.hpp` into your Visual Studio project and you're ready to go.
- **Beautiful Console UI:** Clean ASCII art, colors, and interactive menus out-of-the-box.
- **Hardware Binding:** Automatically grabs system HWID securely.
- **Background Heartbeat:** The SDK spins up a native Windows thread to ping the server every 30 seconds, instantly terminating the process (`ExitProcess`) if the user is banned or their subscription expires.

## Project Structure
```text
├── Findexauthexample/
│   ├── findexauth.hpp         <-- The core C++ SDK header. Do not modify.
│   ├── main.cpp               <-- The interactive Console application.
│   ├── Findexauthexample.sln  <-- Visual Studio Solution
│   └── README.md
```

## Getting Started

### 1. Configure Your App Credentials
Open `main.cpp` in Visual Studio and edit the configuration block at the top of the file:

```cpp
const std::string APP_NAME = "Appliction Name";
const std::string OWNER_ID = "Your-Owner-ID";
const std::string APP_SECRET = "Your-App-Secret";
const std::string APP_VER = "1.0.0";
const std::string SERVER_URL = "https://findexauth.online";
```

### 2. Build the Project
Open `Findexauthexample.sln` in **Visual Studio 2022** (or 2019).
1. Select **Release** or **Debug** mode.
2. Select **x64** (recommended) or **x86**.
3. Go to `Build -> Build Solution`.

### 3. Integration Guide
To add this to your own project, just copy `findexauth.hpp`, include it, and link against `winhttp.lib` (which is done automatically via `#pragma comment` in the header).

```cpp
#include "findexauth.hpp"

int main() {
    FindexAuth::api auth("MyApp", "OwnerID", "AppSecret", "1.0", "https://findexauth.online");
    auth.init();

    if (auth.response.success) {
        auth.login("username", "password");
        
        if (auth.response.success) {
            std::cout << "Welcome " << auth.user_data.username << "!\n";
            auth.start_heartbeat(30);
            
            // Your protected code here
        }
    }
}
```

## Migrating from KeyAuth
If you are migrating an existing project from KeyAuth, switching to FindexAuth is seamless and takes only a few minutes.

1. **Remove KeyAuth:** Delete `keyauth.hpp` and any KeyAuth library files (`libcurl.lib`, etc.) from your project.
2. **Add FindexAuth:** Drop `findexauth.hpp` into your project and `#include "findexauth.hpp"`.
3. **Update Initialization:**
   Change your old KeyAuth initialization to FindexAuth:
   ```cpp
   FindexAuth::api auth("MyApp", "OwnerID", "AppSecret", "1.0", "https://findexauth.online");
   auth.init();
   ```
4. **Update API Calls:** The function names are nearly identical:
   - `KeyAuthApp.login(user, pass)` ➔ `auth.login(user, pass)`
   - `KeyAuthApp.register(user, pass, key)` ➔ `auth.register_key(key, user, pass)`
   - `KeyAuthApp.license(key)` ➔ `auth.license(key)`
   - `KeyAuthApp.check()` ➔ `auth.start_heartbeat(30)` (Runs continuously in the background!)
5. **Remove Dependencies:** Unlike KeyAuth, FindexAuth relies entirely on native Windows APIs (`WinHTTP`). You can safely remove all the bulky `libcurl`, `openssl`, and `nlohmann/json` dependencies from your project!

## Troubleshooting
- **Failed to connect (WinHTTP open failed):** Ensure your internet connection is active.
- **Linker Errors (LNK2019):** Make sure your project links against `winhttp.lib` and `Advapi32.lib`. The provided `findexauth.hpp` handles this automatically for MSVC.

---
*Built with ❤️ for FindexAuth.*
