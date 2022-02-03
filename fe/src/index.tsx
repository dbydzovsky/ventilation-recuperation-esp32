import * as React from "react";
import * as ReactDOM from "react-dom";
import {ReduxRoot} from "./ReduxRoot";
import register from "./registerServiceWorker"

const rootEl = document.getElementById("root");
ReactDOM.render(<ReduxRoot />, rootEl);

register();
