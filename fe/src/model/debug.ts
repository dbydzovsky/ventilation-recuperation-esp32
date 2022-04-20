
export enum DebugActions{
    SET_MESSAGES = "SET_MESSAGES",
}

export interface DebugActionType<T, P> {
    type: T;
    payload: P;
}

export type DebugAction =
    | DebugActionType<typeof DebugActions.SET_MESSAGES , void>;

export interface DebugMessage{
    time: string,
    text: string,
    severity: "NONE" | "WARN" | "ERR"
}