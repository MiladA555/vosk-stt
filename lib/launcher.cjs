async function loadESM() {
	const { pathToFileURL } = require('url');
	const { runVoskRecognition } = await import(pathToFileURL(require.resolve('./launcher.js')));
	return runVoskRecognition;
}

module.exports = {
	runVoskRecognition: (...args) => loadESM().then(fn => fn(...args))
};