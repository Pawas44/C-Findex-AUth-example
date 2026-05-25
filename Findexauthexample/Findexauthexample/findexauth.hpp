#pragma once
#include <windows.h>
#include <winhttp.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>
#include <sddl.h>
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "Advapi32.lib")

namespace FindexAuth {

// ── HTTP helpers ──────────────────────────────────────────────────────────────
static std::string http_post(const std::string& full_url, const std::string& body) {
    std::string host, path;
    bool use_ssl = false;
    INTERNET_PORT port = 80;
    auto proto_end = full_url.find("://");
    std::string rest = (proto_end != std::string::npos) ? full_url.substr(proto_end + 3) : full_url;
    if (full_url.size() >= 5 && full_url.substr(0,5) == "https") { use_ssl = true; port = 443; }
    auto path_pos = rest.find('/');
    if (path_pos == std::string::npos) { host = rest; path = "/"; }
    else { host = rest.substr(0, path_pos); path = rest.substr(path_pos); }
    auto colon = host.find(':');
    if (colon != std::string::npos) { port = (INTERNET_PORT)std::stoi(host.substr(colon+1)); host = host.substr(0,colon); }
    std::wstring whost(host.begin(),host.end()), wpath(path.begin(),path.end());
    HINTERNET hSess = WinHttpOpen(L"FindexAuth/2.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0);
    if(!hSess) return "{\"success\":false,\"message\":\"WinHTTP open failed\"}";
    HINTERNET hConn = WinHttpConnect(hSess, whost.c_str(), port, 0);
    DWORD flags = use_ssl ? WINHTTP_FLAG_SECURE : 0;
    HINTERNET hReq = WinHttpOpenRequest(hConn, L"POST", wpath.c_str(), nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
    std::wstring ct = L"Content-Type: application/json";
    WinHttpAddRequestHeaders(hReq, ct.c_str(), (DWORD)-1, WINHTTP_ADDREQ_FLAG_ADD);
    WinHttpSendRequest(hReq, WINHTTP_NO_ADDITIONAL_HEADERS, 0, (LPVOID)body.c_str(), (DWORD)body.size(), (DWORD)body.size(), 0);
    WinHttpReceiveResponse(hReq, nullptr);
    std::string resp; DWORD avail = 0;
    while(WinHttpQueryDataAvailable(hReq,&avail) && avail>0) {
        std::vector<char> buf(avail+1,0); DWORD read=0;
        WinHttpReadData(hReq, buf.data(), avail, &read);
        resp.append(buf.data(), read);
    }
    WinHttpCloseHandle(hReq); WinHttpCloseHandle(hConn); WinHttpCloseHandle(hSess);
    return resp.empty() ? "{\"success\":false}" : resp;
}

static std::string http_get(const std::string& full_url) {
    std::string host, path; bool use_ssl=false; INTERNET_PORT port=80;
    auto proto_end=full_url.find("://");
    std::string rest=(proto_end!=std::string::npos)?full_url.substr(proto_end+3):full_url;
    if(full_url.size()>=5&&full_url.substr(0,5)=="https"){use_ssl=true;port=443;}
    auto path_pos=rest.find('/');
    if(path_pos==std::string::npos){host=rest;path="/";}
    else{host=rest.substr(0,path_pos);path=rest.substr(path_pos);}
    auto colon=host.find(':');
    if(colon!=std::string::npos){port=(INTERNET_PORT)std::stoi(host.substr(colon+1));host=host.substr(0,colon);}
    std::wstring whost(host.begin(),host.end()),wpath(path.begin(),path.end());
    HINTERNET hSess=WinHttpOpen(L"FindexAuth/2.0",WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,WINHTTP_NO_PROXY_NAME,WINHTTP_NO_PROXY_BYPASS,0);
    HINTERNET hConn=WinHttpConnect(hSess,whost.c_str(),port,0);
    DWORD flags=use_ssl?WINHTTP_FLAG_SECURE:0;
    HINTERNET hReq=WinHttpOpenRequest(hConn,L"GET",wpath.c_str(),nullptr,WINHTTP_NO_REFERER,WINHTTP_DEFAULT_ACCEPT_TYPES,flags);
    WinHttpSendRequest(hReq,WINHTTP_NO_ADDITIONAL_HEADERS,0,WINHTTP_NO_REQUEST_DATA,0,0,0);
    WinHttpReceiveResponse(hReq,nullptr);
    std::string resp; DWORD avail=0;
    while(WinHttpQueryDataAvailable(hReq,&avail)&&avail>0){
        std::vector<char> buf(avail+1,0);DWORD read=0;
        WinHttpReadData(hReq,buf.data(),avail,&read);resp.append(buf.data(),read);
    }
    WinHttpCloseHandle(hReq);WinHttpCloseHandle(hConn);WinHttpCloseHandle(hSess);
    return resp;
}

// ── HWID ──────────────────────────────────────────────────────────────────────
static std::string get_hwid() {
    HANDLE hToken = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) return "UNKNOWN";
    DWORD dwLength = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwLength);
    if (dwLength == 0) { CloseHandle(hToken); return "UNKNOWN"; }
    PTOKEN_USER pTokenUser = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), 0, dwLength);
    if (pTokenUser == NULL) { CloseHandle(hToken); return "UNKNOWN"; }
    if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwLength, &dwLength)) {
        HeapFree(GetProcessHeap(), 0, pTokenUser); CloseHandle(hToken); return "UNKNOWN";
    }
    LPSTR sidString = NULL;
    std::string sid = "UNKNOWN";
    if (ConvertSidToStringSidA(pTokenUser->User.Sid, &sidString)) {
        sid = sidString;
        LocalFree(sidString);
    }
    HeapFree(GetProcessHeap(), 0, pTokenUser);
    CloseHandle(hToken);
    return sid;
}

// ── Simple JSON helpers ───────────────────────────────────────────────────────
static std::string esc(const std::string& s) {
    std::string out;
    for(char c:s){if(c=='"')out+="\\\"";else if(c=='\\')out+="\\\\";else out+=c;}
    return out;
}
static std::string jobj(std::initializer_list<std::pair<std::string,std::string> > fields) {
    std::string s="{"; bool first=true;
    for(const auto& p:fields){
        if(!first)s+=","; first=false;
        s+="\""+p.first+"\":\""+esc(p.second)+"\"";
    }
    return s+"}";
}
static std::string json_str(const std::string& json, const std::string& key) {
    std::string pat="\""+key+"\":\"";
    auto pos=json.find(pat);
    if(pos==std::string::npos)return "";
    pos+=pat.size();
    std::string val;
    for(;pos<json.size()&&json[pos]!='"';pos++){
        if(json[pos]=='\\'&&pos+1<json.size()){pos++;val+=json[pos];}
        else val+=json[pos];
    }
    return val;
}
static bool json_bool(const std::string& json, const std::string& key) {
    std::string t="\""+key+"\":true", f="\""+key+"\":false";
    return json.find(t)!=std::string::npos;
}

// ── Data classes ──────────────────────────────────────────────────────────────
class userdata{public:std::string username,ip,hwid,expiry,level,license_key,createdate,lastlogin;};
class appdata { public:std::string version, status, numUsers, downloadLink, webloaderLink, hash, updateVersion; };
class responsedata{
public:
    bool success=false,needs_registration=false,username_taken=false;
    std::string message,validated_key;
};

// ── Main API class ─────────────────────────────────────────────────────────────
class api {
public:
    std::string name,ownerid,secret,version,url;
    static long long responseTime;
    userdata    user_data;
    appdata     app_data;
    responsedata response;

    api(std::string name,std::string ownerid,std::string secret,
        std::string version,std::string url)
        :name(name),ownerid(ownerid),secret(secret),version(version),
         url(url.back()=='/'?url.substr(0,url.size()-1):url){}

    void init(){
        auto start=std::chrono::high_resolution_clock::now();
        std::string body=jobj({{"app_name",name},{"app_secret",secret}});
        std::string resp=http_post(url+"/api/1.3/app-info",body);
        auto end=std::chrono::high_resolution_clock::now();
        responseTime=std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        response.success=json_bool(resp,"success");
        response.message=json_str(resp,"message");
        if(!response.success)return;
        app_data.downloadLink =json_str(resp,"download_link");
        app_data.updateVersion=json_str(resp,"latest_version");
        if(!app_data.updateVersion.empty()&&app_data.updateVersion!=version){
            response.success=false;
            response.message="Update available! v"+app_data.updateVersion+" Download: "+
                (app_data.downloadLink.empty()?url+"/update":app_data.downloadLink);
        }
    }

    void license(const std::string& key){
        std::string body=jobj({{"app_name",name},{"app_secret",secret},{"key",key},{"hwid",get_hwid()},{"version",version}});
        std::string resp=http_post(url+"/api/1.3/login-key",body);
        response.success=json_bool(resp,"success");
        response.message=json_str(resp,"message");
        response.needs_registration=json_bool(resp,"needs_registration");
        response.validated_key=json_str(resp,"key");
        if(response.validated_key.empty())response.validated_key=key;
        if(response.success&&!response.needs_registration)_load_user(resp);
    }

    void register_key(const std::string& key,const std::string& username,const std::string& password){
        std::string body=jobj({{"app_name",name},{"app_secret",secret},{"key",key},{"username",username},{"password",password},{"hwid",get_hwid()},{"version",version}});
        std::string resp=http_post(url+"/api/1.3/register-key",body);
        response.success=json_bool(resp,"success");
        response.message=json_str(resp,"message");
        response.username_taken=json_bool(resp,"username_taken");
        if(response.success)_load_user(resp);
    }

    void login(const std::string& username,const std::string& password){
        std::string body=jobj({{"app_name",name},{"app_secret",secret},{"username",username},{"password",password},{"hwid",get_hwid()},{"version",version}});
        std::string resp=http_post(url+"/api/1.3/login",body);
        response.success=json_bool(resp,"success");
        response.message=json_str(resp,"message");
        if(response.success)_load_user(resp);
    }

    bool checkblack(){
        std::string body=jobj({{"app_name",name},{"app_secret",secret},{"hwid",get_hwid()}});
        std::string resp=http_post(url+"/api/1.3/check-blacklist",body);
        return json_bool(resp,"success");
    }

    bool heartbeat(){
        if(user_data.username.empty())return true;
        std::string body=jobj({{"app_name",name},{"app_secret",secret},{"username",user_data.username},{"hwid",get_hwid()}});
        std::string resp=http_post(url+"/api/1.3/heartbeat",body);
        bool ok=json_bool(resp,"success");
        response.message=json_str(resp,"message");
        if(resp=="{\"success\":false}"&&json_str(resp,"reason").empty())return true;
        return ok;
    }

    void start_heartbeat(int interval_seconds=30){
        struct HBData{std::string url,app_name,app_secret,*p_username,hwid;int ms;};
        auto* d=new HBData{url,name,secret,&user_data.username,get_hwid(),interval_seconds*1000};
        CreateThread(nullptr,0,[](LPVOID param)->DWORD{
            auto* d=reinterpret_cast<HBData*>(param);
            while(true){
                Sleep(d->ms);
                if(d->p_username->empty())continue;
                std::string body=jobj({{"app_name",d->app_name},{"app_secret",d->app_secret},{"username",*d->p_username},{"hwid",d->hwid}});
                std::string resp=http_post(d->url+"/api/1.3/heartbeat",body);
                if(!json_bool(resp,"success")&&!json_str(resp,"reason").empty()){
                    std::string msg=json_str(resp,"message");
                    MessageBoxA(nullptr,msg.c_str(),"Session Ended",MB_OK|MB_ICONWARNING|MB_TOPMOST|MB_SETFOREGROUND);
                    ExitProcess(1);
                }
            }
            return 0;
        },d,0,nullptr);
    }

    std::string var(const std::string& varid){
        std::string body=jobj({{"app_name",name},{"app_secret",secret},{"varid",varid}});
        std::string resp=http_post(url+"/api/1.3/var",body);
        return json_bool(resp,"success")?json_str(resp,"value"):"";
    }

    void log(const std::string& msg){
        std::string body=jobj({{"app_name",name},{"app_secret",secret},{"username",user_data.username},{"message",msg}});
        http_post(url+"/api/1.3/log",body);
    }

private:
    void _load_user(const std::string& resp){
        auto extract=[&](const std::string& key)->std::string{
            std::string nested_pat="\"data\":{";
            auto dp=resp.find(nested_pat);
            if(dp!=std::string::npos){
                std::string sub=resp.substr(dp+8);
                int depth=1; size_t i=0;
                for(;i<sub.size()&&depth>0;i++){if(sub[i]=='{')depth++;else if(sub[i]=='}')depth--;}
                std::string v=json_str("{"+sub.substr(0,i),key);
                if(!v.empty())return v;
            }
            return json_str(resp,key);
        };
        user_data.username   =extract("username");
        user_data.hwid       =extract("hwid");
        user_data.ip         =extract("ip");
        user_data.expiry     =extract("expiry");
        user_data.level      =extract("level");
        user_data.license_key=extract("license_key");
        user_data.lastlogin  =extract("last_login");
        user_data.createdate =extract("created_at");
    }
};

long long api::responseTime=0;

} // namespace FindexAuth