#include "format.hpp"
#include <cmath>
#include <iomanip>

namespace util::format {
    std::string formatDateTime(time::system_time_point tp) {
        auto timet = time::sysclock::to_time_t(tp);
        auto nowms = chrono::duration_cast<chrono::milliseconds>(tp.time_since_epoch()) % 1000;

        std::tm time_info = *std::localtime(&timet);

        std::ostringstream oss;
        oss << std::put_time(&time_info, "%Y-%m-%d %H:%M:%S") << '.' << std::setfill('0') << std::setw(3) << nowms.count();

        return oss.str();
    }

    std::string formatBytes(uint64_t bytes) {
        // i did not write this myself
        static const char* suffixes[] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"};

        if (bytes == 0) {
            return "0B";
        }

        int exp = static_cast<int>(std::log2(bytes) / 10);
        double value = static_cast<double>(bytes) / std::pow(1024, exp);

        std::ostringstream oss;

        if (std::fmod(value, 1.0) == 0) {
            oss << static_cast<int>(value);
        } else {
            oss << std::fixed << std::setprecision(1) << value;
        }

        oss << suffixes[exp];

        return oss.str();
    }

    std::string formatErrorMessage(std::string message) {
        if (message.find("<html>") != std::string::npos) {
            message = "<HTML response, not showing>";
        } else if (message.size() > 128) {
            message = message.substr(0, 128) + "...";
        }

        return message;
    }

    std::string formatPlatformerTime(uint32_t ms) {
        auto dur = time::millis(ms);
        auto hours = time::as<time::hours>(dur).count() % 24;
        auto minutes = time::as<time::minutes>(dur).count() % 60;
        auto seconds = time::as<time::seconds>(dur).count() % 60;
        auto millis = ms % 1000;

        if (hours > 0) {
            return fmt::format("{}:{:02}:{:02}.{:03}", hours, minutes, seconds, millis);
        } else if (minutes > 0) {
            return fmt::format("{}:{:02}.{:03}", minutes, seconds, millis);
        } else {
            return fmt::format("{}.{:03}", seconds, millis);
        }
    }

    Result<cocos2d::ccColor3B> parseColor(const std::string_view hex) {
        std::string_view subview;

        size_t hashPos = hex.find('#');
        if (hashPos == std::string::npos) {
            subview = hex;
        } else {
            subview = hex.substr(hashPos + 1);
        }

        auto result = parse<unsigned int>(subview, 16);
        GLOBED_REQUIRE_SAFE(result.has_value(), "failed to parse the hex color")

        int rgb = result.value();

        return Ok(cocos2d::ccColor3B {
            .r = static_cast<uint8_t>((rgb >> 16) & 0xff),
            .g = static_cast<uint8_t>((rgb >> 8) & 0xff),
            .b = static_cast<uint8_t>(rgb & 0xff)
        });
    }

    std::string colorToHex(cocos2d::ccColor3B color) {
        return fmt::format("{:02X}{:02X}{:02X}", color.r, color.g, color.b);
    }

    std::string colorToHex(cocos2d::ccColor4B color) {
        return fmt::format("{:02X}{:02X}{:02X}{:02X}", color.r, color.g, color.b, color.a);
    }

    std::string rtrim(const std::string_view str, const std::string_view filter) {
        size_t start = 0;
        size_t end = str.length();

        while (end > start && filter.find(str[end - 1]) != std::string::npos) {
            --end;
        }

        return std::string(str.substr(0, end));
    }

    std::string ltrim(const std::string_view str, const std::string_view filter) {
        size_t start = 0;
        size_t end = str.length();

        while (start < end && filter.find(str[start]) != std::string::npos) {
            ++start;
        }

        return std::string(str.substr(start));
    }

    std::string trim(const std::string_view str, const std::string_view filter) {
        return ltrim(rtrim(str, filter), filter);
    }

    std::string toLowercase(const std::string_view str) {
        std::string result;
        for (char c : str) {
            result += std::tolower(c);
        }

        return result;
    }

    std::string toUppercase(const std::string_view str) {
        std::string result;
        for (char c : str) {
            result += std::toupper(c);
        }

        return result;
    }
}