#include "articlemodel.h"
#include "articleproxymodel.h"
#include "databasemanager.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QStandardPaths>
#include <QDir>
#include <QLoggingCategory>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    QByteArray localMsg = msg.toLocal8Bit();
    QString functionName;

    switch (type) {
    case QtDebugMsg:
        functionName = "qDebug";
        break;
    case QtInfoMsg:
        functionName = "qInfo";
        break;
    case QtWarningMsg:
        functionName = "qWarning";
        break;
    case QtCriticalMsg:
        functionName = "qCritical";
        break;
    case QtFatalMsg:
        functionName = "qFatal";
        break;
    }

    FILE* stream = (type == QtDebugMsg || type == QtInfoMsg) ? stdout : stderr;
    fprintf(stream, "%s: %s\n",
            functionName.toStdString().c_str(),
            localMsg.constData());
    fflush(stream);

    if (type == QtFatalMsg)
        abort();
}

int main(int argc, char *argv[])
{
    // qputenv("QSG_INFO", "1");
    // qputenv("QSG_VISUALIZE", "overdraw");
    QLoggingCategory::setFilterRules(QStringLiteral("qt.qml.binding.removal.info=true\n"
                                                    "qt.qml.debug=true"));
    QGuiApplication app(argc, argv);
    qInstallMessageHandler(customMessageHandler);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // TODO: remove DatabaseManager
    DatabaseManager& dbManager = DatabaseManager::instance();
    if(!dbManager.openDatabase())
    {
        qFatal() << "Database opening failure...";
        app.exit(-1);
    }

    // qmlRegisterUncreatableType<Category>("SqlSession", 1, 0,"Category", "Can't create type Category in QML");
    qmlRegisterUncreatableType<ArticleModel>("ArticleModel", 1, 0,"ArticleModel", "Can't create type ArticleModel in QML");
    qmlRegisterUncreatableType<ArticleProxyModel>("ArticleModel", 1, 0,"ArticleProxyModel", "Can't create type ArticleProxyModel in QML");

    QVariantList categoryModel = Category::getList();
    engine.rootContext()->setContextProperty("categoryModel", categoryModel);

    ArticleModel articleModel(dbManager.database());
    engine.rootContext()->setContextProperty("articleModel", &articleModel);
    ArticleProxyModel articleProxyModel;
    articleProxyModel.setSourceModel(&articleModel);
    engine.rootContext()->setContextProperty("articleProxyModel", &articleProxyModel);
    const QString picPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/LPF";
    QDir().mkdir(picPath);
    engine.rootContext()->setContextProperty("picPath", picPath);
    engine.loadFromModule("LaPetiteFripeuse", "Main");

    return app.exec();
}
