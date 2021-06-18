#include "main.h"
#include "GrahamScanApp.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    GrahamScanApp *app = new GrahamScanApp(wxT("GrahamScanApp"));
    app->Show(true);

    return true;
}
