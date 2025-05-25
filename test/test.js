import { runVoskRecognition } from '../lib/launcher.js';

(async () => {
	try {
		let result = await runVoskRecognition({modelPath:'../model/english',audioFile:'../media/test.wav'});
		console.log('\n*** Recognition Result ***');
		console.log(JSON.parse(result));
	} catch (err) {
		console.error('Error:', err);
	}
})();
