#include <string>
#include <functional>
#include <algorithm>
#include <opencv2/videoio.hpp>

// debug helpers (may put these in another header later)
#ifdef _DEBUG
#include <iostream>
#include <string_view>
#include <source_location>
#include<opencv2/highgui.hpp>
template <typename ...Args>
void trace(const std::source_location& loc, Args&& ...args) {
    std::ostringstream stream;
    stream << loc.file_name() << "\t" << loc.function_name() << ":" << loc.line() << "\t";
    (stream << ... << std::forward<Args>(args)) << '\n';
    std::cout << stream.str();
}
#define TRACE(...) do { trace(std::source_location::current(), __VA_ARGS__); } while(0)

#define GET_MAT_TYPE(mat_type, type_str) do { \
    switch (mat_type) { \
        case CV_8UC1: \
            type_str = "CV_8UC1"; \
            break; \
        case CV_8UC2: \
            type_str = "CV_8UC2"; \
            break; \
        case CV_8UC3: \
            type_str = "CV_8UC3"; \
            break; \
        case CV_8UC4: \
            type_str = "CV_8UC4"; \
            break; \
        case CV_8SC1: \
            type_str = "CV_8SC1"; \
            break; \
        case CV_8SC2: \
            type_str = "CV_8SC2"; \
            break; \
        case CV_8SC3: \
            type_str = "CV_8SC3"; \
            break; \
        case CV_8SC4: \
            type_str = "CV_8SC4"; \
            break; \
        case CV_16UC1: \
            type_str = "CV_16UC1"; \
            break; \
        case CV_16UC2: \
            type_str = "CV_16UC2"; \
            break; \
        case CV_16UC3: \
            type_str = "CV_16UC3"; \
            break; \
        case CV_16UC4: \
            type_str = "CV_16UC4"; \
            break; \
        case CV_16SC1: \
            type_str = "CV_16SC1"; \
            break; \
        case CV_16SC2: \
            type_str = "CV_16SC2"; \
            break; \
        case CV_16SC3: \
            type_str = "CV_16SC3"; \
            break; \
        case CV_16SC4: \
            type_str = "CV_16SC4"; \
            break; \
        case CV_32SC1: \
            type_str = "CV_32SC1"; \
            break; \
        case CV_32SC2: \
            type_str = "CV_32SC2"; \
            break; \
        case CV_32SC3: \
            type_str = "CV_32SC3"; \
            break; \
        case CV_32SC4: \
            type_str = "CV_32SC4"; \
            break; \
        case CV_32FC1: \
            type_str = "CV_32FC1"; \
            break; \
        case CV_32FC2: \
            type_str = "CV_32FC2"; \
            break; \
        case CV_32FC3: \
            type_str = "CV_32FC3"; \
            break; \
        case CV_32FC4: \
            type_str = "CV_32FC4"; \
            break; \
        case CV_64FC1: \
            type_str = "CV_64FC1"; \
            break; \
        case CV_64FC2: \
            type_str = "CV_64FC2"; \
            break; \
        case CV_64FC3: \
            type_str = "CV_64FC3"; \
            break; \
        case CV_64FC4: \
            type_str = "CV_64FC4"; \
            break; \
        case CV_16FC1: \
            type_str = "CV_16FC1"; \
            break; \
        case CV_16FC2: \
            type_str = "CV_16FC2"; \
            break; \
        case CV_16FC3: \
            type_str = "CV_16FC3"; \
            break; \
        case CV_16FC4: \
            type_str = "CV_16FC4"; \
            break; \
        default: \
            type_str = "unknown"; \
            break; \
    } \
} while (0)

#define IMSHOW(name, frame) do { \
        cv::imshow(name, frame); \
        cv::waitKey(1); \
    } while(0)
#else
#define TRACE(...)
#define GET_MAT_TYPE(mtype, str_ref)
#define IMSHOW(name, frame)
#endif
#pragma once

namespace util {
    struct VideoParameters {
        std::string name;
        int fourcc;
        double fps;
        cv::Size frameSize;
        bool isColor = true;

        VideoParameters(std::string _name, int _fourcc, double _fps, cv::Size _frameSize, bool _isColor = true)
                                        : name{name}, fourcc{_fourcc}, fps{_fps}, frameSize{_frameSize}, isColor{_isColor} {}
        VideoParameters(std::string filename);
        VideoParameters(cv::VideoCapture src);
    };

}
