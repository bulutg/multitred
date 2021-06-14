//
// Created by Bulut Gözübüyük on 4.06.2021.
//

#ifndef SIYAH1_PARSEHELPER_H
#define SIYAH1_PARSEHELPER_H

inline std::tm localtime_xp(std::time_t timer) {
    std::tm bt{};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

// default = "YYYY-MM-DD HH:MM:SS"
inline std::string time_stamp(const std::string &fmt = "%F_%T") {
    auto bt = localtime_xp(std::time(0));
    char buf[64];
    return {buf, std::strftime(buf, sizeof(buf), fmt.c_str(), &bt)};
}

#endif //SIYAH1_PARSEHELPER_H
