import React from 'react';
import {useSelector} from "react-redux";
import {RootState} from "../reducers";
import {makeStyles} from "@material-ui/core/styles";
import {Backdrop, CircularProgress} from "@material-ui/core";

export function Loader() {
    const loader = useSelector((state: RootState) => state.loader);
    const classes = useStyles();

    return <Backdrop className={classes.backdrop} open={loader.loading}>
            <div className="App-logo" style={{width: 100, height: 100}}>
                <svg className="MuiSvgIcon-root innerLoader" style={{width: 70, height: 70}} focusable="false" viewBox="0 0 24 24" aria-hidden="true"
                     role="presentation">
                    <path
                        d="M12 12c0-3 2.5-5.5 5.5-5.5S23 9 23 12H12zm0 0c0 3-2.5 5.5-5.5 5.5S1 15 1 12h11zm0 0c-3 0-5.5-2.5-5.5-5.5S9 1 12 1v11zm0 0c3 0 5.5 2.5 5.5 5.5S15 23 12 23V12z"></path>
                </svg>
        </div>
        {/*<CircularProgress color="inherit" />*/}
    </Backdrop>
}

const useStyles = makeStyles((theme) => ({
    backdrop: {
        zIndex: theme.zIndex.drawer + 1,
        color: '#fff',
    },
}));
