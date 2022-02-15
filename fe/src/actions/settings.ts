import {Configuration, ConfigurationActions} from "../model/configuration";
import {LoaderActions} from "../model/loader";
import {handleResponseWithNotification} from "./notification";
import {Settings, SettingsActions} from "../model/settings";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export function saveSettings(settings: Settings) {
    return async function save(dispatch: Function, getState: Function) {
        dispatch({type: LoaderActions.SHOW_LOADER})
        const body = JSON.stringify(settings);
        let headers = new Headers({
            "Content-Type": "application/json"
        })

        await handleResponseWithNotification(dispatch,
            "Nastavení bylo uloženo",
            "Nebylo možné nastavení uložit",
            () => {
            return fetch(baseurl + "/a/settings/", {method: "POST", body: body, headers: headers});
        })
    }
}
export async function getSettings(dispatch: Function, getState: Function) {
    dispatch({type: LoaderActions.SHOW_LOADER})
    try {
        const response = await fetch(baseurl + "/a/settings/", {method: "GET"});
        const obj = await response.json();
        console.log(JSON.stringify(obj))
        dispatch({type: SettingsActions.GET_SETTINGS, payload: obj})
    } finally {
        dispatch({type: LoaderActions.HIDE_LOADER})
    }

}