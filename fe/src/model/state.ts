export enum StateActions{
  GET_STATE = "GET_STATE",
}

export enum ConnectionState {
  Connected = "Connected",
  Disconnected = "Disconnected",
  Unitiliazed = "Unitiliazed",
  TooMuchRequests = "TooMuchRequests"
}

export interface TempPressSensor {
  tAvg: number
  tErr: number
  tWarn: number
  t: number
}
export interface TempHumSensor {
  tAvg: number
  tErr: number
  tWarn: number
  t: number
  hAvg: number
  hErr: number
  hWarn: number
  h: number
}

export interface CurrentState {
  t: string,
  p: number,
  a: number,
  mo: 0|1|2|3, // 0 inactive, 1 winter, 2 summer, 3 auto
  tpp?: number,
  tpd?: number,
  o: TempHumSensor,
  i: TempPressSensor,
  r: number,
  h: number,
  de: string
}

interface StateActionType<T, P> {
  type: T;
  payload: P;
}

export type StateAction =
    | StateActionType<typeof StateActions.GET_STATE, CurrentState>;
