
Name:		cuneiform-qt
Version:	0.1.1
Release:	alt0.M50.1
Summary:	GUI frontend for Cuneiform OCR

License:	GPL
Group:		Graphics
URL:		http://www.altlinux.org/Cuneiform-Qt

Packager:   	Andrey Cherepanov <cas@altlinux.org>

Source0:	%name-%version.tar.bz2

BuildRequires: gcc-c++ libqt4-devel >= 4.3.0
Requires: cuneiform

%description
This application is GUI frontend for Cuneiform (OCR system originally 
developed and open sourced by Cognitive technologies). It allow
to open scanned image, view this one in preview pane, recornize text via
Cuneiform and save result in HTML file.

%prep
%setup -q
DESTDIR=%buildroot PREFIX=/usr qmake-qt4 %name.pro

%build
%make_build

%install
make install INSTALL_ROOT=%buildroot

%files
%doc AUTHORS README TODO
%_bindir/%name
%_datadir/apps/%name/*.qm
%_desktopdir/%name.desktop
%_pixmapsdir/%name.png

%changelog
* Tue Apr 07 2009 Andrey Cherepanov <cas@altlinux.org> 0.1.1-alt0.M50.1
- Build for 5.0 branch

* Tue Apr 07 2009 Andrey Cherepanov <cas@altlinux.org> 0.1.1-alt1
- Add Russian translation
- Save from text widget instead copy temporary file
- Remove temporary file
- Check for errors during saving
- Differ saved format and filter in saved file selection dialog
- Show application icon in about dialog
- Remove native option because it is binary format and it will not display correctly
- Fix readOnly flag for result text
- Move installation instruction from spec to QMake project file

* Mon Apr 06 2009 Andrey Cherepanov <cas@altlinux.org> 0.1.0-alt1
- Initial release

