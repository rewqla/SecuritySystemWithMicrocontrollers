const express = require('express');
const path = require('path');
const session = require('express-session');
const app = express();
const port = 3000;

app.set('view engine', 'ejs');

app.use(express.static(path.join(__dirname, 'public')));
app.use(express.urlencoded({ extended: true }));
app.use(express.json());

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

let historyLogs = [
    {
        userId: 1,
        deviceName: "Buzzer",
        time: "13:49",
        metadata: "Activated due to proximity threshold"
    },
    {
        userId: 1,
        deviceName: "Distance Sensor",
        time: "13:49",
        metadata: "Detected object at 25 cm"
    }
];

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

    const userHistory = historyLogs.filter(log => log.userId === req.session.userId);
    res.render('history', { history: userHistory });
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
    console.log(req)
    const { username, password } = req.body;

    const existingUser = users.find(user => user.username === username);
    if (existingUser) {
        return res.render('register', { messages: ['Username already exists. Please choose another one.'] });
    }

    const newUserId = users.length + 1;
    users.push({ userId: newUserId, username, password });

    return res.render('login', { messages: ['Registration successful! You can now log in.'] });
});

app.get('/api/configuration', (req, res) => {
    const { serialNumber } = req.query;

    console.log("Received serial number " + serialNumber)
    const configuration = configurations.find(config => config.serialNumber === serialNumber);
    console.log(configuration)

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

app.post('/api/history', (req, res) => {
    const { userId, deviceName, time, metadata } = req.body;

    if (!userId || !deviceName || !time || !metadata) {
        return res.status(400).json({ message: "Missing required fields" });
    }

    const newHistory = {
        userId: parseInt(userId),
        deviceName: deviceName,
        time: time,
        metadata: metadata
    };

    console.log(newHistory)

    historyLogs.push(newHistory);

    res.status(200);
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
