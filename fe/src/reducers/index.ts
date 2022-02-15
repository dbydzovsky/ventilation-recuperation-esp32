import {History} from "history";
import {combineReducers} from "redux";
import * as stateReducer from "./state";
import * as errorReducer from "./error";
import {ErrorState} from "./error";
import * as configurationReducer from "./configuration";
import * as loaderReducer from "./loader";
import * as settingsReducer from "./settings";
import * as notificationReducer from "./notification";
import {CurrentState} from "../model/state";
import {LoaderState} from "../model/loader";
import {NotificationState} from "../model/notification";
import {ConfigurationState} from "../model/configuration";
import {SettingsState} from "../model/settings";

export interface RootState {
	state: CurrentState,
	settings: SettingsState,
	errorState: ErrorState,
	loader: LoaderState,
	notificationState: NotificationState,
	configuration: ConfigurationState,
}
export default (history: History) =>
	combineReducers({
		...stateReducer,
		...errorReducer,
		...loaderReducer,
		...settingsReducer,
		...configurationReducer,
		...notificationReducer,
	});
