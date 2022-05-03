export enum SettingsActions{
  SET_SETTINGS = "SET_SETTINGS",
  CLEAR_SETTINGS = "CLEAR_SETTINGS",
}

export interface Settings {
  recuperationOn: boolean
  ventilatorRevolutions: number
  recuperationRevolutions: number
  checkRecuperationRpm: boolean
  checkVentilatorRpm: boolean
  hideCo2: boolean
  hideInternalTempHum: boolean
  unblockingFansPeriod: number
  ventilatorMaxRpm: number
  recuperationMaxRpm: number
  recuperationWaitForDirectionChange: number
  recuperationCycleDuration: number
  recuperationMode: number,
  recuperationPower: number,
  ventilatorPower: number,
  durationMillis: number,
  tempDisableDuration: number,
  brightness: number
}

export interface SettingsState {
  settings: Settings
  counter: number
  connected: boolean
}

interface SettingsActionType<T, P> {
  type: T;
  payload: P;
}

export type SettingsAction = SettingsActionType<typeof SettingsActions.SET_SETTINGS, Settings>;
export type ClearSettingsActions = SettingsActionType<typeof SettingsActions.CLEAR_SETTINGS, { connected: boolean }>;
