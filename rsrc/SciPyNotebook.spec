Summary: Notebook like editor for NumPy/SciPy.

%define version 0.2.0

License: GPLv2
Group: Development
Name: SciPyNotebook
Prefix: /usr
Provides: scipynotebook
Release: 1
Source: SciPyNotebook-%{version}.tar.gz
URL: https://github.com/downloads/hmatuschek/SciPyNotebook/
Version: %{version}
Buildroot: /tmp/scipynotbookrpm
BuildRequires: cmake, qt4-devel, python-devel
Requires: qt4, python

%description
This program combines the comfort of an editor with the flexibility of a console.

%prep
%setup -q

%build
cmake -DCMAKE_INSTALL_PREFIX=$RPM_BUILD_ROOT/usr
make

%install
rm -fr $RPM_BUILD_ROOT
make install

%clean
rm -rf $RPM_BUILD_ROOT