import {Typography} from "@material-ui/core";
import * as React from "react";
import {Provider} from "react-redux";
import {PersistGate} from "redux-persist/integration/react";
import App from "./App";
import configureStore from "./configureStore";
import {useEffect} from "react";

const { persistor, store } = configureStore();

export function ReduxRoot() {
	useEffect(() => {
		// Update the document title using the browser API
		let el = document.getElementById("loader");
		if (el) {
			el.remove();
		}
	});
	return (
		<Provider store={store}>
			<PersistGate
				persistor={persistor}
			>
				<App />
			</PersistGate>
		</Provider>
	);
}
