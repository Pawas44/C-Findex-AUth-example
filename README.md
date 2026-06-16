# FindexAuth - C++ SDK Example

Welcome! This is the official **FindexAuth C++ Example**. We made this to show you how easily you can add FindexAuth to your native C++ applications or cheats.

This SDK is incredibly lightweight and uses Windows native HTTP libraries. No bloated external dependencies like OpenSSL or cURL are required!

## 🌟 Features
- **Zero Dependencies:** Compiles natively using `<winhttp.h>` and Windows standard libraries. Perfect for internal game cheats.
- **Hidden Server Webhooks:** You can fire Discord webhooks directly from the server. Crackers will never see your Discord URL.
- **Secure File Delivery:** Download files directly into memory as byte arrays. Never let your secret `.dll` or `.sys` payloads touch the hard drive!
- **Auto Hardware Binding:** Automatically grabs the user's HWID using their Windows SID token.
- **Background Heartbeat:** The SDK spins up a background thread to ping the server, automatically exiting the process if the subscription expires or they are banned.

## 🚀 How to Setup

### 1. Configure Your App
Open `main.cpp` and look for the `api` setup. Fill in your details from the FindexAuth Dashboard:

```cpp
std::string name = "Your App Name";
std::string ownerid = "Your-Owner-ID";
std::string secret = "Your-App-Secret";
std::string version = "1.0";
std::string url = "https://findexauth.online";

FindexAuth::api auth(name, ownerid, secret, version, url);
```

### 2. Compile
Open the solution file (`.sln`) in **Visual Studio**. Ensure you compile in `x64` or `x86` depending on your needs. It should build instantly.

## 💻 Code Examples

Here are some examples of what FindexAuth can do for you:

### Setup & Login
```cpp
auth.init();

if (auth.response.success) {
    auth.login("username", "password");
    if (auth.response.success) {
        std::cout << "Welcome, " << auth.user_data.username << "!\n";
        
        // Start heartbeat protection
        auth.start_heartbeat(30);
    }
}
```

### Server-Sided Webhooks
Keep your webhook URLs hidden on the server!
```cpp
// '123' is your Webhook ID from the dashboard
auth.webhook("123", "User injected the cheat successfully!");
```

### Secure File Download
Download payloads safely without writing to disk.
```cpp
// '5' is your File ID from the dashboard
std::vector<unsigned char> file_bytes = auth.download_file("5");

if (!file_bytes.empty()) {
    // You can now manually map (inject) these bytes straight into memory!
    std::cout << "Payload downloaded securely to memory.\n";
}
```

---
*Built with ❤️ by FindexAuth.*
