#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

namespace fs = std::filesystem;

struct Finding {
    std::string id, severity, confidence, file, message, remediation, code;
    std::size_t line{};
};

struct Options {
    fs::path root{"."};
    std::string format{"text"};
    std::unordered_set<std::string> extensions{
        ".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx"};
};

static std::string lower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

static std::string jsonEscape(const std::string& value) {
    std::string out;
    for (const char c : value) {
        switch (c) {
        case '\\': out += "\\\\"; break;
        case '"': out += "\\\""; break;
        case '\n': out += "\\n"; break;
        case '\r': out += "\\r"; break;
        case '\t': out += "\\t"; break;
        default: out += c;
        }
    }
    return out;
}

static bool ignored(const fs::path& path) {
    static const std::unordered_set<std::string> names{
        ".git", ".svn", "build", "cmake-build-debug", "cmake-build-release",
        "node_modules", "vendor", "third_party", "dist", "out"};
    for (const auto& part : path) {
        if (part == ".git" || names.count(lower(part.string())) != 0) return true;
    }
    return false;
}

static void add(std::vector<Finding>& findings, const std::string& id,
                const std::string& severity, const std::string& confidence,
                const fs::path& file, std::size_t line, const std::string& code,
                const std::string& message, const std::string& remediation) {
    findings.push_back({id, severity, confidence, file.generic_string(), message,
                        remediation, code});
}

static void scanLine(const fs::path& file, std::size_t line, const std::string& raw,
                     std::vector<Finding>& findings) {
    const std::string code = std::regex_replace(raw, std::regex("^\\s+|\\s+$"), "");
    if (code.empty() || code.rfind("//", 0) == 0) return;
    const std::string text = lower(code);

    static const std::regex secret(
        R"((password|passwd|api[_-]?key|secret|access[_-]?token|private[_-]?key)\s*=[^;]*(["'][^"']{8,}["']))",
        std::regex::icase);
    static const std::regex unsafe(R"(\b(strcpy|strcat|sprintf|vsprintf|gets)\s*\()",
                                   std::regex::icase);
    static const std::regex shell(R"(\b(system|popen|exec[a-z]*|ShellExecute)\s*\()",
                                  std::regex::icase);
    static const std::regex sql(R"(\b(select|insert|update|delete)\b.*(\+|<<|std::string\s*\())",
                                std::regex::icase);
    static const std::regex crypto(R"(\b(md5|sha1|des|rc4)\b)", std::regex::icase);
    static const std::regex temp(R"((/tmp/|mktemp\s*\())", std::regex::icase);
    static const std::regex tls(R"((verify_peer\s*[,=]\s*false|CURLOPT_SSL_VERIFYPEER\s*,\s*0|CERT_NONE))",
                                std::regex::icase);

    if (std::regex_search(code, secret))
        add(findings, "SEC001", "HIGH", "HIGH", file, line, code,
            "Possible hard-coded credential or secret.",
            "Load secrets from a protected secret manager or environment at runtime; rotate exposed values.");
    if (std::regex_search(code, unsafe))
        add(findings, "SEC002", "HIGH", "HIGH", file, line, code,
            "Unsafe C string function can cause buffer overflow.",
            "Use bounded APIs with explicit size checks, or safer abstractions such as std::string.");
    if (std::regex_search(code, shell))
        add(findings, "SEC003", "HIGH", "MEDIUM", file, line, code,
            "Process or shell execution requires careful input validation.",
            "Avoid shell interpretation; use a fixed executable and an argument vector with allow-listed inputs.");
    if (std::regex_search(code, sql))
        add(findings, "SEC004", "HIGH", "MEDIUM", file, line, code,
            "SQL appears to be constructed through string concatenation.",
            "Use prepared statements and bind parameters instead of concatenating user-controlled data.");
    if (std::regex_search(code, crypto))
        add(findings, "SEC005", "MEDIUM", "MEDIUM", file, line, code,
            "Weak or obsolete cryptographic algorithm detected.",
            "Use a modern, reviewed library and approved algorithms such as SHA-256 or AEAD encryption.");
    if (std::regex_search(code, temp))
        add(findings, "SEC006", "MEDIUM", "MEDIUM", file, line, code,
            "Temporary-file usage may permit symlink or predictable-name attacks.",
            "Use a platform secure temporary-file API with exclusive creation and restrictive permissions.");
    if (std::regex_search(code, tls))
        add(findings, "SEC007", "CRITICAL", "HIGH", file, line, code,
            "TLS certificate or peer verification appears to be disabled.",
            "Enable certificate and hostname verification; never disable it in production.");
}

static int usage(const char* program) {
    std::cout << "Usage: " << program << " --path <directory-or-file> [--format text|json] [--extensions c,cpp,h]\\n";
    return 0;
}

static bool parse(int argc, char** argv, Options& options) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--help" || arg == "-h") return false;
        if (arg == "--path" && i + 1 < argc) { options.root = argv[++i]; continue; }
        if (arg == "--format" && i + 1 < argc) { options.format = lower(argv[++i]); continue; }
        if (arg == "--extensions" && i + 1 < argc) {
            options.extensions.clear();
            std::stringstream ss(argv[++i]); std::string ext;
            while (std::getline(ss, ext, ',')) {
                ext = lower(ext); if (!ext.empty() && ext[0] != '.') ext = "." + ext;
                options.extensions.insert(ext);
            }
            continue;
        }
        throw std::invalid_argument("unknown or incomplete option: " + arg);
    }
    if (options.format != "text" && options.format != "json")
        throw std::invalid_argument("format must be text or json");
    return true;
}

int main(int argc, char** argv) {
    Options options;
    try {
        if (!parse(argc, argv, options)) return usage(argv[0]);
    } catch (const std::exception& error) {
        std::cerr << "error: " << error.what() << "\n"; return 2;
    }
    std::error_code ec;
    if (!fs::exists(options.root, ec)) { std::cerr << "error: path does not exist\n"; return 2; }
    std::vector<Finding> findings;
    auto scan = [&](const fs::path& file) {
        std::ifstream input(file);
        if (!input) { std::cerr << "warning: cannot read " << file << "\n"; return; }
        std::string line; std::size_t number = 0;
        while (std::getline(input, line)) scanLine(file, ++number, line, findings);
    };
    if (fs::is_regular_file(options.root)) scan(options.root);
    else for (const auto& entry : fs::recursive_directory_iterator(
                 options.root, fs::directory_options::skip_permission_denied, ec))
        if (entry.is_regular_file() && !ignored(entry.path()) &&
            options.extensions.count(lower(entry.path().extension().string())) != 0) scan(entry.path());

    if (options.format == "json") {
        std::cout << "{\"findings\":[";
        for (std::size_t i = 0; i < findings.size(); ++i) {
            if (i) std::cout << ','; const auto& f = findings[i];
            std::cout << "{\"id\":\"" << f.id << "\",\"severity\":\"" << f.severity
                      << "\",\"confidence\":\"" << f.confidence << "\",\"file\":\""
                      << jsonEscape(f.file) << "\",\"line\":" << f.line << ",\"message\":\""
                      << jsonEscape(f.message) << "\",\"remediation\":\"" << jsonEscape(f.remediation)
                      << "\",\"code\":\"" << jsonEscape(f.code) << "\"}";
        }
        std::cout << "],\"count\":" << findings.size() << "}\n";
    } else {
        for (const auto& f : findings)
            std::cout << f.severity << " " << f.id << " " << f.file << ":" << f.line
                      << " [" << f.confidence << "]\n  " << f.message << "\n  Fix: " << f.remediation << "\n";
        std::cout << "Scanned security findings: " << findings.size() << "\n";
    }
    return findings.empty() ? 0 : 1;
}
