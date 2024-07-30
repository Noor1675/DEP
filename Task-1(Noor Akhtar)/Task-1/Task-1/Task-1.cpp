#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>

void WeatherForecastingSystem::fetchForecastData(const Location& location) {
    // 1. Build the API URL
    QString apiUrl = "https://api.openweathermap.org/data/2.5/weather?";
    apiUrl += "appid=" + apiKey; // Replace apiKey with your actual key
    apiUrl += "&lat=" + QString::number(location.getLatitude());
    apiUrl += "&lon=" + QString::number(location.getLongitude());

    // 2. Create a QNetworkRequest
    QNetworkRequest request(QUrl(apiUrl));

    // 3. Use QNetworkAccessManager
    QNetworkAccessManager* manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &WeatherForecastingSystem::onForecastDataReceived);

    // 4. Send the request
    manager->get(request);
}

void WeatherForecastingSystem::onForecastDataReceived(QNetworkReply* reply) {
    if (reply->error() == QNetworkReply::NoError) {
        // 1. Get the response data
        QByteArray jsonData = reply->readAll();

        // 2. Parse the JSON
        QJsonDocument doc = QJsonDocument::fromJson(jsonData);
        QJsonObject rootObject = doc.object();

        // 3. Extract weather information 
        QJsonObject mainObject = rootObject.value("main").toObject();
        double temperature = mainObject.value("temp").toDouble();

        // 4. Do something with the data
        // ...
    }
    else {
        // Handle errors
        qDebug() << "Error fetching weather data:" << reply->errorString();
    }
    reply->deleteLater();
}