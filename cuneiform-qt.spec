
Name:		cuneiform-qt
Version:	0.1.0
Release:	alt1
Summary:	GUI frontend for Cuneiform OCR

License:	GPL
Group:		Graphics
URL:		http://www.altlinux.org/Cuneiform-Qt

Packager:   	Andrey Cherepanov <cas@altlinux.org>

Source0:	%name-%version.tar.bz2

BuildRequires: gcc-c++ libqt4-devel
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
lrelease-qt4 %name.pro
%make_build

%install
%makeinstall
#install -D -m 0644 *.qm %buildroot%_datadir/apps/%name/
install -D -m 0644 %name.desktop %buildroot%_datadir/applications/%name.desktop
install -D -m 0644 icons/%name.png %buildroot%_datadir/pixmaps/%name.png

%files
%doc AUTHORS README TODO
%_bindir/%name
#%_datadir/apps/%name/
%_datadir/applications/%name.desktop
%_datadir/pixmaps/%name.png

%changelog
* Mon Apr 06 2009 Andrey Cherepanov <cas@altlinux.org> 0.1.0-alt1
- Initial release

