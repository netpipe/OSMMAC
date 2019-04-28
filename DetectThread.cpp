#include "DetectThread.h"
using namespace std;
using namespace cv;

DetectThread::DetectThread(string sample_Path,string detect_Path,cv::Mat frame):QThread()
{
    viewResult = false;
    samplePath = sample_Path;
    detectPath = detect_Path;
    cameraData = frame;
    dollars =0;
}
void DetectThread::run()
{
    dollars =0;
    Mat img1,img2,img3;
    size_t max_val = 8;
    Mat max_image;
    vector<KeyPoint> max_kp;
    string max_file;
    vector<DMatch> temp_matches;

    // mask
    cv::Point corners[1][4];
    corners[0][0] = Point( 10, 10 );
    corners[0][1] = Point( 20, 20 );
    corners[0][2] = Point( 30, 10 );
    corners[0][3] = Point( 20, 10 );
    const Point* corner_list[1] = { corners[0] };
    int num_points = 4;
    int num_polygons = 1;
    int line_type = 8;
    Mat mask(30,30,CV_8UC3, cv::Scalar(0,0,0));
    fillPoly( mask, corner_list, &num_points, num_polygons, cv::Scalar( 255, 255, 255 ),  line_type);

    // Match between img1 and img2
    // keypoint  for img1 and img2
    vector<KeyPoint> keyImg1, keyImg2;
    // Descriptor for img1 and img2
    Mat descImg1, descImg2;
    const float ratio_thresh = 0.789f;

    Ptr<Feature2D> b = ORB::create();
    Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE_HAMMING);

    vector<String> imgDetect,imgSample;

    try
    {
        glob(samplePath + "/*.jpg", imgSample);

        if(detectPath.substr(detectPath.size()-4,1)==".")
        {
            imgDetect.push_back(detectPath);
        }
        else
        {
            glob(detectPath + "/*.jpg", imgDetect);
        }
    }catch (Exception& e)
    {
         cout << e.msg << endl;
    }


    for (size_t k = 0; k < imgDetect.size(); k++)
    {
        max_val = 8;

        try
        {
            if(imgDetect[k].substr(imgDetect[k].size()-10,6)=="camera")
            {
                img1 = cameraData;
            }
            else
            {
                img1 = imread(imgDetect[k], IMREAD_COLOR);
            }

        } catch (Exception& e) {
            cout<<e.msg<<endl;
        }

            cout<<""<<imgDetect[k]<<"----";
            if (img1.rows*img1.cols <= 0)
                {
                cout << "Image " << imgDetect[k] << " is empty or cannot be found\n";
                continue;
                }
            resize(img1, img1,Size(1800,825));
            Rect roi(704, 711, 301, 41);
            Mat crop = img1(roi);
            imwrite("temp.jpg", crop);
//            imshow("Good Matches: ", crop );
            QProcess process;
            process.start("tesseract temp.jpg temp");


    //        Mat ROI(img1, Rect(0,0,img1.cols/2,img1.rows));
    //        ROI.copyTo(img1);

            for (size_t i = 0; i < imgSample.size(); i++)
            {
                try
                {
                    img2 = imread(imgSample[i], IMREAD_COLOR);


                    if (img2.rows*img2.cols <= 0)
                    {
                        cout << "Image " << imgSample[i] << " is empty or cannot be found\n";
                        continue;
                    }

           //         Mat ROI2(img2, Rect(0,0,img2.cols/2,img2.rows));
          //          ROI2.copyTo(img2);

                    resize(img1, img1,img2.size());
                  //  if (img1.cols>img1.rows) resize(img1, img1,img2.size());

                    b->detectAndCompute(img1, Mat(),keyImg1, descImg1);
                    b->detectAndCompute(img2, Mat(),keyImg2, descImg2);
                    vector< vector<DMatch> > knnmatches;
                    descriptorMatcher->knnMatch(descImg1, descImg2, knnmatches, 2);
                    //-- Filter matches using the Lowe's ratio test
                    vector<DMatch> good_matches;

                    for (size_t j = 0; j < knnmatches.size(); j++)
                    {
                        if (knnmatches[j][0].distance < ratio_thresh * knnmatches[j][1].distance)
                        {
                            good_matches.push_back(knnmatches[j][0]);
                        }
                    }
                    size_t match_count=good_matches.size();
                    cout<<match_count<<"   ";

//                    Mat img_matches;
//                    drawMatches( img1, keyImg1, img2, keyImg2,good_matches, img_matches);
//                    namedWindow("Good Matches: ", WINDOW_GUI_NORMAL);
//                    imshow("Good Matches: ", img_matches );
//                    waitKey();

                    if (max_val <match_count)
                    {
                        temp_matches = good_matches;
                        max_val = match_count;
                        max_image = img2;
                        max_kp = keyImg2;
                        max_file = imgSample[i];
                    }

                }catch (Exception& e)
                    {
                         cout << e.msg << endl;
                    }
            }

            if (max_val > 15)
            {

                try
                {

                    std::string temp=max_file.substr(max_file.find_last_of('/')+1,max_file.find_last_of('.')-max_file.find_last_of('/')-1);
                    std::string numstr="";
                    cout<<"max:"<<temp<<"    ";

                    for(int i=0;i<temp.length();i++)
                        if (isdigit(temp[i]))
                            numstr+=temp[i];
                        else
                            break;
                    int dollar=stoi(numstr);

                    if (viewResult == true)
                    {
                        Mat img_matches;
                        drawMatches( img1, keyImg1, max_image, max_kp,temp_matches, img_matches);
                        namedWindow("Good Matches: "+to_string(dollar)+"dollar", WINDOW_GUI_NORMAL);
                        imshow("Good Matches: "+to_string(dollar)+"dollar", img_matches );
                        waitKey();
                    }
                    cout<<"dollars: "<<dollar<<endl;
                    dollars+=dollar;
                    emit result(dollars);
                }
                catch (Exception& e)
                {
                    cout<<e.msg<<endl;
                }

            }else
            {
                cout<<"No Match"<<endl;
            }
            QFile file("temp.txt");
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&file);
                QString serialNum =in.readLine();
                emit serial(serialNum);

            }
    }
}
