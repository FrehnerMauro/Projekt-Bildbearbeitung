#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

// Color constants
const cv::Scalar color_orange(0, 127, 255);

cv::Mat gray;
cv::Mat gray_old;

bool check_motion(cv::Mat old, cv::Mat check)
{
    // Überprüfen, ob das alte Bild initialisiert ist
    if (old.empty())
        return false;

    // Absolute Differenz zwischen den Bildern berechnen
    cv::Mat diff;
    cv::absdiff(old, check, diff);

    // Schwellenwert für die Differenz festlegen
    int threshold = 30;

    // Mittelwert der Differenz berechnen
    cv::Scalar meanDiff = cv::mean(diff);

    // Überprüfen, ob die Differenz größer als der Schwellenwert ist
    return (meanDiff.val[0] > threshold || meanDiff.val[1] > threshold || meanDiff.val[2] > threshold);
}
    
void clip(cv::Mat& img, int threshold)
{
    img.forEach<uchar>([threshold](uchar& pixel, const int* position) {
        if (pixel < threshold)
            pixel = 0;
    });
}

int main()
{
    // Öffnen der Kamera
    VideoCapture cap(1); // 0 steht für die Indexnummer der Kamera, normalerweise die erste Kamera

    // Überprüfen, ob die Kamera erfolgreich geöffnet wurde
    if (!cap.isOpened())
    {
        cout << "Fehler beim Öffnen der Kamera!" << endl;
        return -1;
    }
    

    // Fenster für die Anzeige erstellen
    namedWindow("Kamera", WINDOW_NORMAL);

    // Schleife zum Lesen und Anzeigen der Kamerabilder
    while (true)
    {
        Mat frame;
        // Kamerabild lesen
        cap >> frame;
        
        
        std::cout<< frame.cols << frame.rows << std::endl;
        
        // Überprüfen, ob ein Bild gelesen wurde
        if (frame.empty())
        {
            cout << "Leeres Bild von der Kamera erhalten!" << endl;
            break;
        }
        
        // Graustufenbild für die Kreiserkennung
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        
        // Normalisieren der Grauwerte (optional)
        cv::normalize(gray, gray, 0, 255, cv::NORM_MINMAX);
        
        // Bewegung überprüfen
        if (!check_motion(gray_old, gray) )
        {
            // Abschneiden der Helligkeitswerte
            clip(gray, 225);
            
            // Kreiserkennung mit dem Hough-Kreis-Algorithmus
            std::vector<cv::Vec3f> circles;
            cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                             gray.rows / 70,  // Mindestabstand zwischen den Mittelpunkten der erkannten Kreise
                             200,  // Schwellenwert für die Canny-Kanten-Erkennung
                             10,   // Schwellenwert für die Abstimmung auf den Radius
                             6,   // Mindest- und maximale Radius des erkannten Kreises
                             10   // Maximale Radius
                             );
            
            // Kreise auf dem Bild markieren
            cv::Mat result = frame.clone();
            for (const auto& circle : circles)
            {
                cv::Point center(std::round(circle[0]), std::round(circle[1]));
                int radius = std::round(circle[2]);
                cv::circle(result, center, radius, cv::Scalar(0, 255, 0), 2);
            }
            
            // Anzahl der Kreise zählen
            int Anzahl_Kreise = circles.size();
            
            // Text für die Ausgabe vorbereiten
            std::string text = "Es wurde eine " + std::to_string(Anzahl_Kreise) + " gewuerfelt";
            
            // Text auf das Bild zeichnen
            cv::putText(result, text, cv::Point(20, 500), cv::FONT_HERSHEY_PLAIN, 3, color_orange, 4);
            
            // Kamerabild anzeigen
            imshow("Kamera", result);
        }
        
        // Bild lokal speichern
        gray_old = gray.clone();
        
        // Auf eine Tastatureingabe warten (Exit mit 'q')
        char key = waitKey(1);
        if (key == 'q') {
            break;
        }
    }

    // Kamera-Objekt freigeben und
    cap.release();
    destroyAllWindows();

    return 0;
}
