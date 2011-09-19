// necessary detection (fixes for <IE9)
mejs.MediaFeatures = {
	init: function() {
		var
			nav = mejs.PluginDetector.nav,
			ua = mejs.PluginDetector.ua.toLowerCase(),
			i,
			v,
			html5Elements = ['source','track','audio','video'];

		// detect browsers (only the ones that have some kind of quirk we need to work around)
		this.isiPad = (ua.match(/ipad/i) !== null);
		this.isiPhone = (ua.match(/iphone/i) !== null);
		this.isAndroid = (ua.match(/android/i) !== null);
		this.isBustedAndroid = (ua.match(/android 2\.[12]/) !== null);
		this.isIE = (nav.appName.toLowerCase().indexOf("microsoft") != -1);
		this.isChrome = (ua.match(/chrome/gi) !== null);
		this.isFirefox = (ua.match(/firefox/gi) !== null);

		// create HTML5 media elements for IE before 9, get a <video> element for fullscreen detection
		for (i=0; i<html5Elements.length; i++) {
			v = document.createElement(html5Elements[i]);
		}
		
		this.supportsMediaTag = (typeof v.canPlayType !== 'undefined' || this.isBustedAndroid);

		// detect native JavaScript fullscreen (Safari only, Chrome fails)
		this.hasNativeFullScreen = (typeof v.webkitRequestFullScreen !== 'undefined');
		if (this.isChrome) {
			this.hasNativeFullScreen = false;
		}
		// OS X 10.5 can't do this even if it says it can :(
		if (this.hasNativeFullScreen && ua.match(/mac os x 10_5/i)) {
			this.hasNativeFullScreen = false;
		}			
	}
};
mejs.MediaFeatures.init();
