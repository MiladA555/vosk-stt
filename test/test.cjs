const { runVoskRecognition } = require('../lib/launcher.cjs');

(async () => {
	try {
		const result = await runVoskRecognition({
			modelPath: '../model/english',
			audioFile: '../media/test.wav'
		});
		
		console.log('\n*** Recognition Result ***');
		console.log(typeof result === 'string' ? JSON.parse(result) : result);
		
	} catch (err) {
		console.error('Error:', err.message || err);
	}
})();