const express = require('express');
const path = require('path');
const session = require('express-session');
const app = express();
const port = 3000;

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({ extended: true }));

app.use(session({
    secret: 'secret-key',
    resave: false,
    saveUninitialized: true
}));

const users = [
    { userId: 1, username: 'admin', password: '1234' }
];

let configurations = [{
    userId: 1,
    serialNumber: '51170740-312f-4c81-bc33-997c220cba83',
    enabledDevices: ['buzzer'],
    distanceThreshold: 30,
    startTime: '13:48',
    endTime: '13:51'
}];

app.get('/', (req, res) => {
    res.render('login', { messages: [] });
});
app.get('/register', (req, res) => {
    res.render('register', { messages: [] });
});

app.get('/cabinet', (req, res) => {
    if (!req.session.userId) {
        return res.redirect('/');
    }

    const userConfig = configurations[req.session.userId - 1] || {};

    res.render('cabinet', { config: userConfig, message: "" });
});

app.get('/history', (req, res) => {
    if (!req.session.userId) {
        return res.redirect('/');
    }

    res.render('history');
});


app.post('/login', (req, res) => {
    const { username, password } = req.body;

    const user = users.find(user => user.username === username && user.password === password);
    if (user) {
        req.session.userId = user.userId;
        res.redirect('/cabinet');
    } else {
        res.render('login', { messages: ['Invalid credentials'] });
    }
});

app.post('/register', (req, res) => {
    const { username, password } = req.body;

    const existingUser = users.find(user => user.username === username);
    if (existingUser) {
        res.render('register', { messages: ['Username already exists. Please choose another one.'] });
    }

    const newUserId = users.length + 1;
    users.push({ userId: newUserId, username, password });

    res.render('login', { messages: ['Registration successful! You can now log in.'] });
});

app.get('/api/configuration', (req, res) => {
    const { serialNumber } = req.query;

    console.log("Received serial number " + serialNumber)
    console.log(configurations)
    const configuration = configurations.find(config => config.serialNumber === serialNumber);

    if (configuration) {
        res.json(configuration);
    } else {
        res.status(404).json({ message: "Configuration not found" });
    }
});

app.post('/submit-cabinet', (req, res) => {
    if (!req.session.userId) {
        return res.redirect('/');
    }

    const { serialNumber, enabledDevices, startTime, endTime, distanceThreshold } = req.body;
    const enabledDevicesArray = Array.isArray(enabledDevices) ? enabledDevices : (enabledDevices === undefined ? [] : [enabledDevices]);

    const userConfig = {
        userId: req.session.userId,
        serialNumber: serialNumber,
        enabledDevices: enabledDevicesArray,
        distanceThreshold: distanceThreshold,
        startTime: startTime,
        endTime: endTime
    };
    configurations[req.session.userId - 1] = userConfig;

    res.render('cabinet', { config: userConfig, message: 'Configuration saved successfully!' });
});

app.get('/logout', (req, res) => {
    req.session.destroy((err) => {
        if (err) {
            return res.redirect('/cabinet');
        }
        res.redirect('/');
    });
});

app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
