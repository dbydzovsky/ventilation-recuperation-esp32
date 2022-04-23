import {LoaderActions} from "../model/loader";
import {SettingsActions} from "../model/settings";
import {DebugActions, DebugMessage} from "../model/debug";
import {Simulate} from "react-dom/test-utils";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export function getDebugMessagesAsync(props: { onDone: () => void}) {
    return async function getDebugMessages(dispatch: Function, getState: Function) {
        // dispatch({type: LoaderActions.SHOW_LOADER})
        try {
            const state = getState();
            const storedVersion = state.debugState.version;
            const versionResponse = await fetch(baseurl + "/a/debugv/", {method: "GET"});
            const versionResponseObj = await versionResponse.json();
            const actualVersion  = Number(versionResponseObj["version"]);
            if (storedVersion === actualVersion) {
                props.onDone();
                return;
            }
            const response = await fetch(baseurl + "/a/debug/", {method: "GET"});
            const obj = await response.json();
            const version = Number(obj["version"]);
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
            dispatch({type: DebugActions.SET_MESSAGES, payload: payload})
            dispatch({type: DebugActions.SET_VERSION, payload: version})
        } finally {
            props.onDone()
            // dispatch({type: LoaderActions.HIDE_LOADER})
        }
    }

}
