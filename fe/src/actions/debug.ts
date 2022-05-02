import {LoaderActions} from "../model/loader";
import {SettingsActions} from "../model/settings";
import {DebugActions, DebugMessage, DebugVersions} from "../model/debug";
import {Simulate} from "react-dom/test-utils";
import {handleResponseWithNotification} from "./notification";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export function setTrace(props: {trace: boolean}) {
    return async function configure(dispatch: Function) {
        const body = JSON.stringify(props);
        let headers = new Headers({
            "Content-Type": "application/json"
        })
        return handleResponseWithNotification(dispatch, "Nastaveno", "Nepodařilo se nastavit", () => {
            return fetch(baseurl + "/a/trace/", {method: "POST", body, headers})
        })
    }
}

export function getDebugMessagesAsync(props: { onDone: () => void}) {
    return async function getDebugMessages(dispatch: Function, getState: Function) {
        // dispatch({type: LoaderActions.SHOW_LOADER})
        try {
            const state = getState();
            const storedVersion = state.debugState.version;
            const versionResponse = await fetch(baseurl + "/a/debugv/", {method: "GET"});
            const versionResponseObj = await versionResponse.json();
            const actualVersion = Number(versionResponseObj["version"]);
            if (storedVersion === actualVersion) {
                return;
            }
            const response = await fetch(baseurl + "/a/debug/", {method: "GET"});
            const obj = await response.json();
            const version = Number(obj["version"]);
            const appVersion = obj["appVersion"];
            const trace = Boolean(obj["trace"]);
            const messages = obj.messages as string[];
            const payload:DebugMessage[] =[];

            messages.filter((it) => {return it !== null && it !== undefined && it !== ""}).forEach((message) => {
                let date = ""
                let severity: "NONE" |"WARN" |"ERR" = "NONE"
                let restMessage = message
                if (message.match(/^\d/)) {
                    let [dateTemp, ...rest] = message.split(' ')
                    date = dateTemp
                    restMessage = rest.join(' ')
                }
                if (restMessage.startsWith("WARN ") || restMessage.startsWith("ERR ")) {
                    let [severityTemp, ...rest] = restMessage.split(' ')
                    severity = severityTemp as "WARN" | "ERR"
                    restMessage = rest.join(' ')
                }
                payload.push({
                    time: date,
                    text: restMessage,
                    severity: severity
                })
            })
            let versions: DebugVersions = {
                version, appVersion, trace
            }
            dispatch({type: DebugActions.SET_MESSAGES, payload: payload})
            dispatch({type: DebugActions.SET_VERSION, payload: versions})
        } finally {
            props.onDone()
            // dispatch({type: LoaderActions.HIDE_LOADER})
        }
    }

}
