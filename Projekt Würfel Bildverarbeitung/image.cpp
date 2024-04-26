//
//  image.cpp
//  Projekt Würfel Bildverarbeitung
//
//  Created by Mauro Frehner on 07.03.2024.
//

#include <opencv2/opencv.hpp>
#include <iostream>


int main(void)
{
    
    // Pfad zu Datei
    std::string Datei_Pfad = "/Users/maurofrehner/Library/CloudStorage/OneDrive-Persönlich/Mauro/Schule/FHGraubünden/Programme/C++ Programme/einlesen abspeichern/moon1.png";
    
    
    // Read external image file and copy into image
    cv::Mat Moon = cv::imread(Datei_Pfad);
   
    
    // Überprüfen, ob das Bild erfolgreich geladen wurde
    if (Moon.empty()) {
        std::cerr << "Fehler beim Laden des Bildes." << std::endl;
        return 1;
    }
    
    
    cv::imshow("Moon", Moon);
    
    // Warten, bis eine Taste gedrückt wird
    cv::waitKey(0);
}
