.hpp.moc :
	if test ! -e $(@F) ; then $(MOC) -o $(@F) $^ ; else : ; fi

.qrc.cpp :
	if test ! -e $(@F) ; then $(RCC) -name resources -o $(@F) $^ ; else : ; fi

.ui.hh :
	if test ! -e $(@F) ; then $(UIC) -o $(@F) $^ ; else : ; fi

BUILT_SOURCES = \
	cheatObject.moc \
	dlgApuChannels.moc \
	dlgCheats.moc \
	dlgInput.moc \
	dlgOverscanBorders.moc \
	dlgStdPad.moc \
	dlgUncomp.moc \
	mainWindow.moc \
	pStyle.moc \
	sbarWidget.moc \
	screenWidget.moc \
	settingsObject.moc

BUILT_SOURCES += \
	designer/application.cpp

BUILT_SOURCES += \
	designer/application.hh \
	designer/dlgOverscanBorders.hh \
	designer/dlgApuChannels.hh \
	designer/dlgStdPad.hh \
	designer/dlgCheats.hh \
	designer/dlgUncomp.hh \
	designer/dlgInput.hh

noinst_LIBRARIES = libgui.a
libgui_a_SOURCES = \
	application.hh \
	cheatObject.cpp \
	cheatObject.hpp \
	dlgApuChannels.cpp \
	dlgApuChannels.hpp \
	dlgCheats.cpp \
	dlgCheats.hpp \
	dlgInput.cpp \
	dlgInput.hpp \
	dlgOverscanBorders.cpp \
	dlgOverscanBorders.hpp \
	dlgStdPad.cpp \
	dlgStdPad.hpp \
	dlgUncomp.cpp \
	dlgUncomp.hpp \
	mainWindow.cpp \
	mainWindow.hpp \
	pStyle.cpp \
	pStyle.hpp \
	qt.cpp \
	qt.h \
	sbarWidget.cpp \
	sbarWidget.hpp \
	screenWidget.cpp \
	screenWidget.hpp \
	settings.cpp \
	settings.h \
	settingsObject.cpp \
	settingsObject.hpp
	
if LINUX
libgui_a_SOURCES += \
	linux/jstick.c \
	linux/jstick.h \
	linux/os_linux.h

if ENAB_QT5
AM_CXXFLAGS = -fPIC
endif

endif

if WINDOWS
libgui_a_SOURCES += \
	windows/jstick.c \
	windows/jstick.h \
	windows/os_windows.h \
	windows/win.h
endif
