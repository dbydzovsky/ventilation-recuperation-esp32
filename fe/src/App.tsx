// prettier-ignore
import {
	AppBar,
	Divider,
	Drawer as DrawerMui, FormControlLabel, FormGroup,
	Hidden,
	IconButton,
	List,
	ListItem,
	ListItemIcon,
	ListItemText, Switch,
	Toolbar,
	Typography,
	useMediaQuery
} from "@material-ui/core";
import {Theme} from "@material-ui/core/styles";
import MenuIcon from "@material-ui/icons/Menu";
import {makeStyles} from "@material-ui/styles";
import * as React from "react";
import {Route, Router} from "react-router-dom";
import {history} from "./configureStore";
import {withRoot} from "./withRoot";
import SettingsIcon from '@material-ui/icons/Settings';
import {CurrentStatePanel, CurrentStateRefresher, VentilatorIcon} from "./pages/CurrentStatePanel";
import {ConfigurationPage} from "./pages/ConfigurationPage";
import {Loader} from "./components/Loader";
import {NotificationWidget} from "./pages/NotificationPage";
import TuneIcon from '@material-ui/icons/Tune';
import WarningIcon from '@material-ui/icons/Warning';
import CheckCircleIcon from '@material-ui/icons/CheckCircle';
import {HelpPage} from "./pages/HelpPage";
import HelpIcon from '@material-ui/icons/Help';
import {CurrentStatePage} from "./pages/CurrentStatePage";
import HomeIcon from '@material-ui/icons/Home';
import {useSelector} from "react-redux";
import {RootState} from "./reducers";
import {TrialPage} from "./pages/TrialPage";
import {FirmwarePage} from "./pages/FirmwarePage";
import {MaintenancePage} from "./pages/MaintenancePage";
import {SettingsLoader, SettingsPage} from "./pages/SettingsPage";
import SettingsEthernetIcon from '@material-ui/icons/SettingsEthernet';
import SystemUpdateAltIcon from '@material-ui/icons/SystemUpdateAlt';
import {UpdatePage} from "./pages/UpdatePage";
import {TestingPage} from "./pages/TestingPage";
// https://v4.mui.com/components/material-icons/#material-icons
import PlayArrowIcon from '@material-ui/icons/PlayArrow';
function Routes() {
	const classes = useStyles();

	return (
		<div className={classes.content}>
			<Loader/>
			<SettingsLoader/>
			<NotificationWidget/>
			<Route exact={true} path="/" component={CurrentStatePage} />
			<Route exact={true} path="/state" component={CurrentStatePage} />
			<Route exact={true} path="/configuration" component={ConfigurationPage} />
			<Route exact={true} path="/advanced" component={SettingsPage} />
			<Route exact={true} path="/maintenance" component={MaintenancePage} />
			<Route exact={true} path="/trial" component={TrialPage} />
			<Route exact={true} path="/help" component={HelpPage} />
			<Route exact={true} path="/firmware" component={FirmwarePage} />
			<Route exact={true} path="/update" component={UpdatePage} />
			<Route exact={true} path="/testing" component={TestingPage} />
			{/*<Route exact={true} path="/scheme" component={SchemePage} />*/}
		</div>
	);
}

function Drawer(props: { toggle: () => void }) {
	const classes = useStyles();
	const cleaningNeeded  = useSelector((state: RootState) => state.state.filterVentilator.needCleaning || (state.state.recuperationEnabled && state.state.filterRecuperation.needCleaning));
	const alarmAttentionNeeded  = useSelector((state: RootState) => state.state.alarmVentilator.needAttention || (state.state.recuperationEnabled && state.state.alarmRecuperation.needAttention));
	const settingsValid = useSelector((state: RootState) => state.state.settingsValid);
	const configValid = useSelector((state: RootState) => state.state.configValid);
	const testingMode = useSelector((state: RootState) => state.state.testingMode);
	const onClick= (path: string) => {
		props.toggle();
		history.push(path);
	}
	return (
		<div>
			<div className={classes.drawerHeader} />
			<Divider />
			<CurrentStateRefresher/>
			<CurrentStatePanel/>
			<Divider />
			<Divider />
			<List>
				<ListItem button onClick={() => onClick("/state")}>
					<ListItemIcon>
						<HomeIcon />
					</ListItemIcon>
					<ListItemText primary="Přehled" />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => onClick("/configuration")}>
					<ListItemIcon>
						{(!configValid) && <WarningIcon htmlColor={"orange"}/> || <SettingsIcon />}
					</ListItemIcon>
					<ListItemText primary="Nastavení" />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => onClick("/maintenance")}>
					<ListItemIcon>
						{(cleaningNeeded||alarmAttentionNeeded) && <WarningIcon htmlColor={"orange"}/> || <CheckCircleIcon />}
					</ListItemIcon>
					<ListItemText primary={(cleaningNeeded||alarmAttentionNeeded) ? "Údržba (zkontrolovat)" : "Údržba"} />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => onClick("/trial")}>
					<ListItemIcon>
						<TuneIcon />
					</ListItemIcon>
					<ListItemText primary="Manuální ovládání" />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => onClick("/help")}>
					<ListItemIcon>
						<HelpIcon />
					</ListItemIcon>
					<ListItemText primary="Nápověda" />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => onClick("/advanced")}>
					<ListItemIcon>
						{(!settingsValid) && <WarningIcon htmlColor={"orange"}/> || <SettingsEthernetIcon />}
					</ListItemIcon>
					<ListItemText primary="Pokročilé" />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => onClick("/firmware")}>
					<ListItemIcon>
						<SystemUpdateAltIcon />
					</ListItemIcon>
					<ListItemText primary="Firmware" />
				</ListItem>
			</List>
			<Divider />
			{testingMode && <>
				<List>
					<ListItem button onClick={() => onClick("/testing")}>
						<ListItemIcon>
							<PlayArrowIcon />
						</ListItemIcon>
						<ListItemText primary="Testing" />
					</ListItem>
				</List>
				<Divider />
			</>}
			{/*<List>*/}
			{/*	<ListItem button onClick={() => history.push("/scheme")}>*/}
			{/*		<ListItemIcon>*/}
			{/*			<SchoolIcon />*/}
			{/*		</ListItemIcon>*/}
			{/*		<ListItemText primary="Schéma" />*/}
			{/*	</ListItem>*/}
			{/*</List>*/}
			{/*<List>*/}
			{/*	<ListItem button onClick={() => history.push("/simulation")}>*/}
			{/*		<ListItemIcon>*/}
			{/*			<KeyboardIcon />*/}
			{/*		</ListItemIcon>*/}
			{/*		<ListItemText primary="Simulation" />*/}
			{/*	</ListItem>*/}
			{/*</List>*/}
			{/*<Divider />*/}
		</div>
	);
}

function App(props: {isLight: boolean, toggleMode: () => void}) {
	const classes = useStyles();
	const [mobileOpen, setMobileOpen] = React.useState(false);
	const isMobile = useMediaQuery((theme: Theme) =>
		theme.breakpoints.down("sm")
	);

	const handleDrawerToggle = () => {
		setMobileOpen(!mobileOpen);
	};

	const name = useSelector((state: RootState) => state.configuration).conf?.name || "rekuperace";
	return (
		<Router history={history}>
			<div className={classes.root}>
				<div className={classes.appFrame}>
					<AppBar className={classes.appBar}>
						<Toolbar>
							<IconButton
								color="inherit"
								aria-label="open drawer"
								onClick={handleDrawerToggle}
								className={classes.navIconHide}
							>
								<MenuIcon />
							</IconButton>
							<Typography
								variant="h6"
								color="inherit"
								noWrap={isMobile}
								className={classes.title}
							>
								<VentilatorIcon percentage={1}/>
								{!isMobile && <div className={classes.titleName}>Solární vzduchový kolektor: </div>}
								<div className={classes.titleName}>{name}</div>

							</Typography>
							{!isMobile && <>
								<span style={{flex: "1 1 auto"}}></span>
								<FormGroup>
									<FormControlLabel control={
										<Switch defaultChecked={!props.isLight} onChange={props.toggleMode}/>
									} label="Dark mode" />
								</FormGroup>
							</>
							}
						</Toolbar>
					</AppBar>
					<Hidden mdUp>
						<DrawerMui
							variant="temporary"
							anchor={"left"}
							open={mobileOpen}
							classes={{
								paper: classes.drawerPaper,
							}}
							onClose={handleDrawerToggle}
							ModalProps={{
								keepMounted: true, // Better open performance on mobile.
							}}
						>
							<Drawer toggle={handleDrawerToggle} />
							{ isMobile && <>
								<FormGroup>
									<FormControlLabel control={
										<Switch onChange={props.toggleMode}/>
									} label="Dark mode" />
								</FormGroup>
							</>}
						</DrawerMui>
					</Hidden>
					<Hidden smDown>
						<DrawerMui
							variant="permanent"
							open
							classes={{
								paper: classes.drawerPaper + " hideOnPrint",
							}}
						>
							<Drawer toggle={handleDrawerToggle} />
						</DrawerMui>
					</Hidden>
					<Routes />
				</div>
			</div>
		</Router>
	);
}


const drawerWidth = 240;
const useStyles = makeStyles((theme: Theme) => ({
	root: {
		width: "100%",
		height: "100%",
		zIndex: 1,
		overflow: "hidden",
	},
	appFrame: {
		position: "relative",
		display: "flex",
		width: "100%",
		height: "100%",
	},
	appBar: {
		zIndex: theme.zIndex.drawer + 1,
		background: theme.palette.primary.dark,
		position: "absolute",
	},
	navIconHide: {
		[theme.breakpoints.up("md")]: {
			display: "none",
		},
	},
	drawerHeader: {
		...theme.mixins.toolbar,
		background: "#1c2d6c",
	},
	drawerPaper: {
		width: 250,
		backgroundColor: theme.palette.background.default,
		[theme.breakpoints.up("md")]: {
			width: drawerWidth,
			position: "relative",
			height: "100%",
		},
	},
	content: {
		backgroundColor: theme.palette.background.default,
		width: "100%",
		position: "relative",
		height: "calc(100% - 56px)",
		marginTop: 56,
		[theme.breakpoints.up("sm")]: {
			height: "calc(100% - 64px)",
			marginTop: 64,
		},
	},
	title: {
		alignContent: "center",
		alignItems: "center",
		display: "flex",
	},
	titleName: {
		padding: 5
	}
}));

export default withRoot(App);
