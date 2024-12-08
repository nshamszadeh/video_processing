Demo of pixel sorting: https://www.youtube.com/watch?v=GRVSnG4T3tc

I am back in school working towards a masters in ece. As such, this project is put on temporary hold, although I do plan on returning to it in the future.
Work has been done in integrating machine learning based image inpainting and maskrcnn for accurate object detection and masking.
Work has also been done to begin integrating GPU acceleration for the image processing algorithms.
Future plans are to use the FFMPEG library for more fine grained video and image processing tasks.

Overall goals for this project are to implement various image/video processing effects I find interesting and aesthetically nice to look at into a portable C++ library.
Effects are applied based on a binary mask on the pixels for each video frame. These masks may be generated to detect objects, edges, or other things. Effects are then applied to the video frames that overlap with the generated mask.

