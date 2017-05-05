#include "seekablevideocapture.h"

/**
 * @brief seekableVideoCapture::seekableVideoCapture
 * @param videoFileName
 * @param ringbuffLength
 */
seekableVideoCapture::seekableVideoCapture(char *videoFileName, int32_t buffLength){
    capture.open(videoFileName);
    assert(capture.isOpened());
    cv::Mat frame;
    for(int32_t i=0;i<buffLength;++i){
        capture >> frame;
        buff.push_back(frame);
    }
    //framePosition = 0;
    buffLen = buffLength;
}

bool seekableVideoCapture::getFrame(int32_t position, cv::Mat &frame){
    int32_t lengthOfPrefetch = position - capture.get(CV_CAP_PROP_POS_FRAMES) + buffLen/2;
    if(lengthOfPrefetch > 0){
        //先頭の不要データを削除しておく
        int32_t lengthOfErase = lengthOfPrefetch + buff.size() - buffLen;
        if(0<=lengthOfErase){
            if(buffLen <= lengthOfErase){
                buff.clear();
            }else{
                buff.erase(buff.begin(),buff.begin()+lengthOfErase);
            }
        }

        //prefetch
        for(int32_t i=0;i<lengthOfPrefetch;++i){
            if(capture.get(CV_CAP_PROP_POS_FRAMES)<capture.get(CV_CAP_PROP_FRAME_COUNT)){
                buff.push_back(cv::Mat());
                capture >> buff.back();
            }else{
                break;
            }
        }

        std::cout << "buff.size()" << buff.size() << std::endl;


    }

    int32_t readFramePositionOfDeque = position - (capture.get(CV_CAP_PROP_POS_FRAMES)-buff.size());
    if((0<=readFramePositionOfDeque)&&(readFramePositionOfDeque<buff.size())){
        frame = buff[readFramePositionOfDeque];
        return true;
    }else{
        frame = cv::Mat();
        return false;
    }
}

bool seekableVideoCapture::getFrameForMIP(int32_t position, cv::Mat &frame){
    int32_t readFramePositionOfDeque = position - (capture.get(CV_CAP_PROP_POS_FRAMES)-buff.size());
    if((0<=readFramePositionOfDeque)&&(readFramePositionOfDeque<buff.size())){
        frame = buff[readFramePositionOfDeque];
        return true;
    }else{
        frame = cv::Mat();
        return false;
    }
}

/*seekableVideoCapture::seekableVideoCapture()
{

}*/