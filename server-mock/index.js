// {
//     "configuration": [
//     { "id": 1, "maxPower": 40, "author": "typicode" }
// ],
//     "state": [
//     { "id": 1, "body": "some comment", "postId": 1 }
// ],
//     "profile": { "name": "typicode" }
// }
//https://github.com/typicode/json-server
let recuperationEnabled = false;
module.exports = () => {
    const data = {
        settings: {
            recuperationOn: recuperationEnabled,
            ventilatorRevolutions: 3,
            recuperationRevolutions: 2,
            checkRecuperationRpm: true,
            maxVentilatorTemp: 70,
            "checkVentilatorRpm": true,
            "hideCo2": false,
            "hideInternalTempHum": false,
            "unblockingFansPeriod": 172800000,
            "ventilatorMaxRpm": 8200,
            "recuperationMaxRpm": 6000,
            "recuperationWaitForDirectionChange": 6000,
            "recuperationCycleDuration": 70000,
            "recuperationMode": 1,
            "recuperationPower": 100,
            "ventilatorPower": 30,
            "durationMillis": 300000000,
            "tempDisableDuration": 10800000,
            "brightness": 100
        },
        debug: {
            messages: [
                "08:50 WARN Loading Settings Json",
                "09:24 ERR Validated Settings JSON",
                "09:24 Error with recuperation",
                "09:26 Filter needs to be cleaner",
            ],
            version: 0,
            appVersion: "2.0.0-dev",
            trace: false
        },
        debugv: {
            version: 0,
            appVersion: "2.0.0-dev",
            trace: false
        },
        conf: {
            "mode": 3,
            "winterMaxInsideTemp": 29,
            "summerMinInsideTemp": 19,
            "winterOnRules": [{"t": 35, "p": 30}, {"t": 40, "p": 70}],
            "summerOnRules": [{"t": 20, "p": 30}, {"t": 10, "p": 0}],
            "name": "rekuperace",
            "weatherApiKey": "9f151bec449ef6f558ce7b1af4f5dcba",
            "lat": "50.25",
            "lon": "15.39",
            "autoWinterStart": 250,
            "autoWinterEnd": 50,
            "autoSummerStart": 100,
            "autoSummerEnd": 200,
            "monitoring": {"feed": "683493479138901944", "key": "590f37c41a71c6febdafe6fc70d4310eb653c45594"}
        },
        t: {},
        s: {
            "trial": {
                "enabled": true,
                "duration": 60000,
                "mode": 1,
                "ventilator": 70,
                "recuperation": 60,
                "recuperationMode": 1
            },
            "filterVentilator": {
                needCleaning: false,
                remainingMinutes: 90
            },
            "filterRecuperation": {
                needCleaning: true,
                remainingMinutes: -5
            },
            alarmVentilator: {
                blocked: false,
                remainMinutes: 95,
                highRpm: false,
                overHeated: true,
                needAttention: false
            },
            alarmRecuperation: {
                blocked: false,
                remainMinutes: 5,
                highRpm: false,
                needAttention: false
            },
            "mode": 3,
            "recuperationEnabled": recuperationEnabled,
            "time": "2022/2/6 9:23.24",
            "ventilator": 0,
            "recuperation": -60,
            "alive": 4082775,
            "description": "xxxx",
            "heap": 207348,
            "insideTemp": {"v": 20, "avg": 20, "err": 0, "warn": 0},
            "insideHum": {"v": 20, "avg": 20, "err": 0, "warn": 0},
            "co2Inside": {"v": 1000, "avg": 1000, "err": 0, "warn": 0},
            "outsideTemp": {"v": 20, "avg": 20, "err": 0, "warn": 0},
            "outsideHum": {"v": 5, "avg": 5, "err": 0, "warn": 0}
        }
    }

    return data
}