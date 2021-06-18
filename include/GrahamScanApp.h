#include <wx/wx.h>
#include <wx/event.h>
#include <vector>
#include "Predicates.h"
#include <list>


class GrahamScanApp : public wxFrame
{
public:
    GrahamScanApp(const wxString& title);

private:
    int const horizontalRes = 1920;
    int const verticalRes = 1080;

    //bitmap resolution should be divisible by 8
    int const bitmapHor = 1920;
    int const bitmapVert = 950;
    wxStaticBitmap *sb;
    wxButton *convHullButton;

    int const desiredPointQuantity = 100000;
    wxVector<wxPoint> pointArray;
    std::list<wxPoint> convexHull;

    void GenerateBitmapField(char bits[], int pixelQuantity, int desiredPointQuantity);
    void GrahamScan(wxVector<wxPoint> pointArray);
    bool IsRightRotation(wxPoint a, wxPoint b, wxPoint c);

    void OnButtonPressed(wxCommandEvent& event);
};

class PointSorterByAngle{
wxPoint basePoint;
public:
    PointSorterByAngle(wxPoint point){ basePoint = point;}
    bool operator() (wxPoint b, wxPoint c);
};
