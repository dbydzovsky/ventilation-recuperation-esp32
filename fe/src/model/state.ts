export enum StateActions{
  GET_STATE = "GET_STATE",
}

export enum ConnectionState {
  Connected = "Connected",
  Disconnected = "Disconnected",
  Unitiliazed = "Unitiliazed",
  TooMuchRequests = "TooMuchRequests"
}

export interface Sensor {
  v?: number
  avg?: number
  err?: number
  warn?: number
}

export interface Trial {
  enabled: boolean,
  duration: number
  mode: number
  ventilator: number
  recuperation: number
  recuperationMode: number
}

export interface AlarmState {
  needAttention: boolean
  blocked: boolean
  highRpm: boolean
  remainMinutes: number
}

export interface FilterState {
  needCleaning: boolean
  remainingMinutes: number
}

export interface CurrentState {
  filterVentilator: FilterState,
  filterRecuperation: FilterState,
  alarmRecuperation: AlarmState,
  alarmVentilator: AlarmState,
  time: string,
  trial: Trial,
  alive: number,
  mode: 0|1|2|3, // 0 inactive, 1 winter, 2 summer, 3 auto
  ventilator: number,
  ventilatorRPM: number,
  recuperation?: number,
  recuperationRPM: number,
  insideTemp?: Sensor,
  insideHum?: Sensor,
  outsideTemp?: Sensor,
  outsideHum?: Sensor,
  co2Inside?: Sensor,
  restarts?: number,
  heap: number,
  description: string
}

interface StateActionType<T, P> {
  type: T;
  payload: P;
}

export type StateAction =
    | StateActionType<typeof StateActions.GET_STATE, CurrentState>;
