#include "QCameraGrab.h"
#include "CameraGrabApp.h"
#include "QSingleMutex.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QSingleMutex single("QCameraGrab");
	if (!single.IsSignal())
	{
		QApplication app(argc, argv);
		QMessageBox;
		return app.exec();
	}

	QCameraGrabApp a(argc, argv);
	QCameraGrab w;
	w.show();
	return a.exec();
}
