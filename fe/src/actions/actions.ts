import {handleResponseWithNotification} from "./notification";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export interface ActionData {
    id: number
}

export async function restartESP(dispatch: Function, getState: Function) {
    generic(dispatch, {id: 1}, {ok: "Úspěšný restart", notok: "Úspěšný restart."})
}
export async function resetI2C(dispatch: Function, getState: Function) {
    generic(dispatch, {id: 2}, {ok: "Reset komunikační linky proběhl", notok: "Nepovedlo se spustit reset komunikační linky."})
}
export async function resetSensor(dispatch: Function, getState: Function) {
    generic(dispatch, {id: 3}, {ok: "Restart sensoru byl spuštěn", notok: "Nepovedlo se spustit restart sensoru."})
}
function generic(dispatch: Function, data: ActionData, labels: {ok: string, notok: string}) {
    handleResponseWithNotification(dispatch, labels.ok, labels.notok,() => {
        const body = JSON.stringify(data);
        let headers = new Headers({
            "Content-Type": "application/json"
        })
        return fetch(baseurl + "/a/a/", {method: "POST", body: body, headers: headers});
    })
}