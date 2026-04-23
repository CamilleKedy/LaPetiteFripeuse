cmake_minimum_required(VERSION 3.16)

project(LaPetiteFripeuse VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Quick Graphs Sql Concurrent)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appLaPetiteFripeuse
    main.cpp
)

target_include_directories(appLaPetiteFripeuse
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dao/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/dto/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/model/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/repository/include
)

qt_policy(SET QTP0004 NEW)

qt_add_qml_module(appLaPetiteFripeuse
    URI LaPetiteFripeuse
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/AppWindow.qml
        qml/SideBar.qml
        qml/Header.qml
        qml/DashboardPage.qml
        qml/StatTile.qml
        qml/Tile.qml
        qml/OrdersPage.qml
        qml/InventoryPage.qml
        qml/AnalyticsPage.qml
        qml/InventoryTile.qml
        qml/ArticleTile.qml
        qml/Utilities/UComboBox.qml
        qml/Utilities/UTextField.qml
        qml/Utilities/UTableView.qml
        qml/Utilities/UMenu.qml
        qml/Utilities/UHorizontalHeaderView.qml
    SOURCES
        databasemanager.h
        dto/include/sqlsession.h
        dao/include/basedao.h
        dao/include/articledao.h
        dao/include/articlevariantdao.h
        dao/include/supplierdao.h
        dao/include/saledao.h
        dao/include/salearticlevariantdao.h
        dao/include/subcategorydao.h



        repository/include/articlerepository.h
        repository/include/salerepository.h
        model/include/articlemodel.h
        databasemanager.cpp
        dto/src/sqlsession.cpp
        dao/src/basedao.cpp
        dao/src/articledao.cpp
        dao/src/articlevariantdao.cpp
        dao/src/supplierdao.cpp
        dao/src/saledao.cpp
        dao/src/salearticlevariantdao.cpp
        dao/src/subcategorydao.cpp



        repository/src/articlerepository.cpp
        repository/src/salerepository.cpp
        model/src/articlemodel.cpp
    RESOURCES
        resources.qrc
        SOURCES repository/include/supplierrepository.h
        SOURCES repository/src/supplierrepository.cpp
        SOURCES model/include/articleproxymodel.h
        SOURCES model/src/articleproxymodel.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appLaPetiteFripeuse PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appLaPetiteFripeuse
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appLaPetiteFripeuse
    PRIVATE Qt6::Quick
    PRIVATE Qt6::Graphs
    PRIVATE Qt6::Sql
    PRIVATE Qt6::Concurrent
)

include(GNUInstallDirs)
install(TARGETS appLaPetiteFripeuse
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
