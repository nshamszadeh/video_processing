namespace VideoParameters {
    struct VideoParameters {
	    std::string name;
	    int fourcc;
	    double fps;
	    cv::Size frameSize;
	    bool isColor = true;

	    VideoParameters(std::string _name, int _fourcc, double _fps, cv::size _frameSize, bool _isColor = true)
	                    : name{name}, fourcc{_fourcc}, fps(_fps), frameSize{_frameSize}, isColor{_isColor} {}
    };

VideoParameters getVideoParams(std::string video_path);



}
