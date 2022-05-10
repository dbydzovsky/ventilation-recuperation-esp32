import {CssBaseline, Switch, useMediaQuery} from "@material-ui/core";
import {createMuiTheme} from "@material-ui/core/styles";
import {ThemeProvider} from "@material-ui/styles";
import * as React from "react";
import useCookie from "./util/useCookie";

// A theme with custom primary and secondary color.
// It's optional.

export function withRoot(Component: any) {
	function WithRoot(props: object) {
		const prefersDarkMode = useMediaQuery('(prefers-color-scheme: dark)');
		const initialMode = prefersDarkMode ? 'dark' : 'light';
		const [mode, setMode] = useCookie("ventilation.uimode", initialMode);
		let theme = React.useMemo(() => {
			return createMuiTheme({
				palette: {
					type: mode as 'dark' | 'light',
					primary: {
						light: "#e5e5e5",
						main: "#727272",
						dark: "#363839",
						contrastText: "#fff",
					},
					secondary: {
						light: "#ff5e50",
						main: "#e41e26",
						dark: "#a90000",
						contrastText: "#fff",
					},
				},
			})
		}, [mode]);
		const isLight = mode === 'light';
		const toggle = () => setMode(isLight ? 'dark' : 'light')
		return (
			<ThemeProvider theme={theme}>
				{/* Reboot kickstart an elegant, consistent, and simple baseline to build upon. */}
				<CssBaseline />
				<Component {...props} isLight={isLight} toggleMode={toggle} />
			</ThemeProvider>
		);
	}

	return WithRoot;
}
