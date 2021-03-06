Summary: Notebook like editor for NumPy/SciPy.

%define version 1.0.0rc2

License: GPLv2
Group: Development
Name: scipynotebook
Prefix: /usr
Release: 1
Source: scipynotebook-%{version}.tar.gz
URL: https://github.com/downloads/hmatuschek/SciPyNotebook/
Version: %{version}
Buildroot: /tmp/scipynotebookrpm
BuildRequires: gcc-c++, cmake, libqt4-devel, python-devel
Requires: qt4, python

%description
This program combines the comfort of an editor with the flexibility of a console.

%prep
%setup -q

%build
cmake -DCMAKE_INSTALL_PREFIX=$RPM_BUILD_ROOT/usr
make

%install
make install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-, root, root, -)
%doc README.rst LICENSE
/usr/bin/scipynotebook
/usr/share/applications/scipynotebook.desktop
/usr/share/icons/scipynotebook.png
