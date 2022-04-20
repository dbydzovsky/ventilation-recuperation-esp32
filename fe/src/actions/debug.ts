import {LoaderActions} from "../model/loader";
import {SettingsActions} from "../model/settings";
import {DebugActions, DebugMessage} from "../model/debug";
import {Simulate} from "react-dom/test-utils";

const baseurl = process.env.NODE_ENV === "development" ? "http://localhost:5000" : "";

export function getDebugMessagesAsync(props: { onDone: () => void}) {
    return async function getDebugMessages(dispatch: Function, getState: Function) {
        // dispatch({type: LoaderActions.SHOW_LOADER})
        try {
            const response = await fetch(baseurl + "/a/debug/", {method: "GET"});
            const obj = await response.json();
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
        } finally {
            props.onDone()
            // dispatch({type: LoaderActions.HIDE_LOADER})
        }
    }

}
