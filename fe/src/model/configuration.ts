export enum ConfigurationActions{
  GET_CONFIGURATION = "GET_CONFIGURATION",
}

export interface Rule {
  tv: number
  p: number
}

export interface Monitoring {
  feed: string,
  key: string
}

export enum Mode {
  INACTIVE = 0,
  WINTER = 1,
  SUMMER = 2,
  AUTO = 3,
}

export interface Configuration {
  name: string
  mode: Mode
  winterMaxInsideTemp: number
  winterOnRules: Rule[]
  summerOnRules: Rule[]
  co2Rules: Rule[]
  summerMinInsideTemp: number
  minimumFeelsLike: number
  monitoring: Monitoring
  weatherApiKey: string
  lat: string
  lon: string
  autoWinterStart: number
  autoWinterEnd: number
  autoSummerStart: number
  autoSummerEnd: number
}

export interface ConfigurationState {
  conf: Configuration | undefined
}

interface ConfigurationActionType<T, P> {
  type: T;
  payload: P;
}

export type ConfigurationAction =
    | ConfigurationActionType<typeof ConfigurationActions.GET_CONFIGURATION, Configuration>;