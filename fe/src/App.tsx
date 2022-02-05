// prettier-ignore
import {
	AppBar,
	Divider,
	Drawer as DrawerMui,
	Hidden,
	IconButton,
	List,
	ListItem,
	ListItemIcon,
	ListItemText,
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
import {HelpPage} from "./pages/HelpPage";
import HelpIcon from '@material-ui/icons/Help';
import {ActionPage} from "./pages/ActionsPage";
import {CurrentStatePage} from "./pages/CurrentStatePage";
import HomeIcon from '@material-ui/icons/Home';
import {useSelector} from "react-redux";
import {RootState} from "./reducers";
import {TrialPage} from "./pages/TrialPage";
import ControlCameraIcon from '@material-ui/icons/ControlCamera';

function Routes() {
	const classes = useStyles();

	return (
		<div className={classes.content}>
			<Loader/>
			<NotificationWidget/>
			<Route exact={true} path="/" component={CurrentStatePage} />
			<Route exact={true} path="/state" component={CurrentStatePage} />
			<Route exact={true} path="/configuration" component={ConfigurationPage} />
			<Route exact={true} path="/actions" component={ActionPage} />
			<Route exact={true} path="/trial" component={TrialPage} />
			<Route exact={true} path="/help" component={HelpPage} />
			{/*<Route exact={true} path="/scheme" component={SchemePage} />*/}
		</div>
	);
}

function Drawer(props: {  }) {
	const classes = useStyles();

	return (
		<div>
			<div className={classes.drawerHeader} />
			<Divider />
			<CurrentStateRefresher/>
			<CurrentStatePanel/>
			<Divider />
			<Divider />
			<List>
				<ListItem button onClick={() => history.push("/state")}>
					<ListItemIcon>
						<HomeIcon />
					</ListItemIcon>
					<ListItemText primary="Přehled" />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => history.push("/configuration")}>
					<ListItemIcon>
						<SettingsIcon />
					</ListItemIcon>
					<ListItemText primary="Nastavení" />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => history.push("/trial")}>
					<ListItemIcon>
						<TuneIcon />
					</ListItemIcon>
					<ListItemText primary="Manuální ovládání" />
				</ListItem>
			</List>
			<Divider />
			<List>
				<ListItem button onClick={() => history.push("/help")}>
					<ListItemIcon>
						<HelpIcon />
					</ListItemIcon>
					<ListItemText primary="Nápověda" />
				</ListItem>
			</List>
			<Divider />
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

function App() {
	const classes = useStyles();
	const [mobileOpen, setMobileOpen] = React.useState(true);
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
							<Drawer />
						</DrawerMui>
					</Hidden>
					<Hidden smDown>
						<DrawerMui
							variant="permanent"
							open
							classes={{
								paper: classes.drawerPaper,
							}}
						>
							<Drawer />
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
	drawerHeader: { ...theme.mixins.toolbar },
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
