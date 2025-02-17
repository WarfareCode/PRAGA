# Note: define srcarchivename in Travis build only.
%{!?srcarchivename: %global srcarchivename PRAGA-%{version}}

Name:           PRAGA
Version:        1.3.5
Release:        1%{?dist}
Summary:        PRogram for AGrometeorological Analysis

URL:            https://github.com/ARPA-SIMC/PRAGA
Source0:        https://github.com/ARPA-SIMC/PRAGA/archive/v%{version}.tar.gz#/%{srcarchivename}.tar.gz
License:        GPL

BuildRequires:  qt5-qtbase
BuildRequires:  qt5-devel
BuildRequires:  qt5-qtcharts
BuildRequires:  qt5-qtcharts-devel
BuildRequires:  netcdf
BuildRequires:  netcdf-devel

Requires:       qt5-qtbase-mysql

%description
PRAGA is a geographical application for climatological, meteorological and
agrometeorological analysis. It manages point and gridded dataset, and it
enables gridding point data by using interpolation procedures contained in
agrolib interpolation library. The most relevant agroclimatological variables
are managed (air temperature, precipitation, air relative humidity, solar
radiation, wind intensity and direction, reference evapotranspiration, leaf
wetness). It uses specific widgets and libraries for visualizing data on maps
and graphs.


%prep
%autosetup -n %{srcarchivename}

%build
pushd mapGraphics
qmake-qt5 MapGraphics.pro -spec linux-g++-64 CONFIG+=release CONFIG+=force_debug_info CONFIG+=c++11 CONFIG+=qtquickcompiler
make
popd

pushd makeall
qmake-qt5 makeall.pro -spec linux-g++-64 CONFIG+=release CONFIG+=force_debug_info CONFIG+=c++11 CONFIG+=qtquickcompiler
make qmake_all
make
popd

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p %{buildroot}/%{_bindir}/
cp -a src/PRAGA %{buildroot}/%{_bindir}/

%files
%{_bindir}/PRAGA


%changelog
* Mon Feb  1 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.3.4-1
- Release 1.3.4

* Tue Jan 26 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.3.3-1
- Release 1.3.3

* Wed Jan 20 2021 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.3.2-1
- Release 1.3.2

* Mon Nov 16 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.5-1
- Release 1.2.5

* Fri Nov 13 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.4-1
- Release 1.2.4

* Thu Nov 12 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.3-1
- Release 1.2.3

* Thu Nov  5 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.2.1-1
- Release 1.2.1

* Thu Oct 29 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.1.1-1
- Fixed working directory problem

* Wed Oct 21 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.0-2
- MySQL driver

* Fri Oct  9 2020 Emanuele Di Giacomo <edigiacomo@arpae.it> - 1.0.0-1
- Release 1.0.0
