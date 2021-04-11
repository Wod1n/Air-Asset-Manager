QT += widgets

HEADERS     = mainWindow.h \
              squadronsDialog.h \
              aircraftDialog.h \
              editDialog.h \
              transferDialog.h \
              dependencies/headers/aircraft.h \
              dependencies/headers/location.h \
              dependencies/headers/inventory.h \
              dependencies/headers/shape.h \
              dependencies/headers/diagram.h

FORMS       = mainWindow.ui \
              squadronsDialog.ui \
              aircraftDialog.ui \
              editDialog.ui \
              transferDialog.ui

SOURCES     = mainWindow.cpp \
              squadronsDialog.cpp \
              aircraftDialog.cpp \
              editDialog.cpp \
              transferDialog.cpp \
              dependencies/aircraft.cpp \
              dependencies/location.cpp \
              dependencies/inventory.cpp \
              dependencies/shape.cpp \
              dependencies/diagram.cpp \
              main.cpp

target.path = $$[QT_INSTALL_EXAMPLES]/designer/calculatorform
INSTALLS += target
