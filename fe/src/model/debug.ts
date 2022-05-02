
export enum DebugActions{
    SET_MESSAGES = "SET_MESSAGES",
    SET_VERSION = "SET_VERSION",
}

export interface DebugVersions {
    version: number
    appVersion: string
    trace: boolean
}
export interface DebugActionType<T, P> {
    type: T;
    payload: P;
}

export type DebugAction =
    | DebugActionType<typeof DebugActions.SET_MESSAGES , void>
    | DebugActionType<typeof DebugActions.SET_VERSION , DebugVersions>;

export interface DebugMessage{
    time: string,
    text: string,
    severity: "NONE" | "WARN" | "ERR"
}