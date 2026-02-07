function ultraFastBookingBot() {
    const CONFIG = {
        TARGET_BUS_NUMBER: 'BUS 03',
        MAX_WAIT_TIME: 60000,  // 60 seconds max
        STEP_TIMEOUT: 5000     // 5 seconds per step
    };
    
    // Booking state machine
    const STATE = {
        SEARCHING: 'searching',
        CLICKING_BOOK: 'clicking_book',
        WAITING_FORM: 'waiting_form',
        SUBMITTING: 'submitting',
        COMPLETE: 'complete',
        FAILED: 'failed'
    };
    
    let currentState = STATE.SEARCHING;
    let observer = null;
    let stepTimer = null;
    let globalTimer = null;
    
    // More stable selectors (fallback to multiple strategies)
    const findBusElement = () => {
        const targetBusUpper = CONFIG.TARGET_BUS_NUMBER.toUpperCase();
        
        // Strategy 1: Look for data attributes (most stable)
        let busElements = document.querySelectorAll('[data-bus-number]');
        for (const el of busElements) {
            if (el.dataset.busNumber?.toUpperCase().includes(targetBusUpper)) {
                return el;
            }
        }
        
        // Strategy 2: Look for specific text content
        const allElements = document.querySelectorAll('div[class*="blue"], .bus-item, [class*="bus"]');
        for (const el of allElements) {
            const text = el.textContent?.trim().toUpperCase();
            if (text?.includes(targetBusUpper)) {
                return el;
            }
        }
        
        // Strategy 3: Original selector as fallback
        busElements = document.querySelectorAll('div.bg-gradient-to-r.from-blue-500.to-blue-600.py-2.px-4.text-white');
        for (const el of busElements) {
            const text = el.textContent?.trim().toUpperCase();
            if (text?.includes(targetBusUpper)) {
                return el;
            }
        }
        
        return null;
    };
    
    const findBookButton = (busElement) => {
        if (!busElement) return null;
        
        // Multiple strategies to find book button
        return (
            busElement.parentNode?.querySelector('button[data-action="book"]') ||
            busElement.parentNode?.querySelector('button.bg-green-500') ||
            busElement.parentNode?.querySelector('button:not([disabled])') ||
            busElement.closest('.bus-card, .bus-item')?.querySelector('button.bg-green-500')
        );
    };
    
    const findSubmitButton = () => {
        return (
            document.querySelector('button[data-action="submit"]') ||
            document.querySelector('button[type="submit"]:not([disabled])') ||
            document.querySelector('.modal button[type="submit"]') ||
            document.querySelector('button.bg-blue-500, button.bg-green-500')
        );
    };
    
    // Step 1: Search for BUS 03 using MutationObserver
    function startSearching() {
        console.log('🔍 Searching for', CONFIG.TARGET_BUS_NUMBER);
        currentState = STATE.SEARCHING;
        
        // Immediate check
        const busElement = findBusElement();
        if (busElement) {
            proceedToBooking(busElement);
            return;
        }
        
        // Set up MutationObserver for reactive detection
        observer = new MutationObserver((mutations) => {
            if (currentState !== STATE.SEARCHING) return;
            
            // Check if our target bus appeared
            const busElement = findBusElement();
            if (busElement) {
                observer.disconnect();
                proceedToBooking(busElement);
            }
        });
        
        // Observe the entire document for changes
        observer.observe(document.body, {
            childList: true,
            subtree: true,
            attributes: true,
            attributeFilter: ['class', 'disabled']
        });
    }
    
    // Step 2: Click the Book button
    function proceedToBooking(busElement) {
        console.log('✅ Bus found! Attempting to book...');
        currentState = STATE.CLICKING_BOOK;
        
        const bookButton = findBookButton(busElement);
        
        if (!bookButton || bookButton.disabled) {
            console.log('⚠️ Book button not available');
            setState(STATE.FAILED, 'Book button not clickable');
            return;
        }
        
        // Click the book button
        bookButton.click();
        console.log('🎯 Clicked book button');
        
        // Immediately move to next step
        requestAnimationFrame(() => {
            waitForForm();
        });
    }
    
    // Step 3: Wait for form/modal to appear
    function waitForForm() {
        console.log('⏳ Waiting for booking form...');
        currentState = STATE.WAITING_FORM;
        
        let attempts = 0;
        const maxAttempts = 50; // 50 * 100ms = 5 seconds
        
        const checkForForm = () => {
            attempts++;
            
            const submitButton = findSubmitButton();
            
            if (submitButton && !submitButton.disabled) {
                console.log('📝 Form ready!');
                proceedToSubmit(submitButton);
                return;
            }
            
            if (attempts >= maxAttempts) {
                console.log('❌ Form did not appear in time');
                setState(STATE.FAILED, 'Form timeout');
                return;
            }
            
            // Use requestAnimationFrame for fastest checking
            requestAnimationFrame(checkForForm);
        };
        
        requestAnimationFrame(checkForForm);
    }
    
    // Step 4: Submit the form
    function proceedToSubmit(submitButton) {
        console.log('🚀 Submitting booking...');
        currentState = STATE.SUBMITTING;
        
        // Auto-select first stop if dropdown exists
        const stopSelect = document.querySelector('select[name="stop"], select#stop, .stop-select');
        if (stopSelect && stopSelect.options.length > 1) {
            stopSelect.selectedIndex = 1; // Select first real option (0 is usually placeholder)
            console.log('📍 Selected stop:', stopSelect.options[1]?.text);
        }
        
        // Submit the form
        submitButton.click();
        console.log('✅ Booking submitted!');
        
        setState(STATE.COMPLETE, 'Booking successful!');
    }
    
    // State management
    function setState(newState, message) {
        currentState = newState;
        console.log(`🔄 State: ${newState} - ${message}`);
        
        cleanup();
        
        if (newState === STATE.COMPLETE) {
            console.log('🎉 BOOKING COMPLETE!');
        } else if (newState === STATE.FAILED) {
            console.log('💔 Booking failed:', message);
        }
    }
    
    // Cleanup function
    function cleanup() {
        if (observer) {
            observer.disconnect();
            observer = null;
        }
        if (stepTimer) {
            clearTimeout(stepTimer);
            stepTimer = null;
        }
        if (globalTimer) {
            clearTimeout(globalTimer);
            globalTimer = null;
        }
    }
    
    // Initialize bot
    function init() {
        console.log('🤖 Ultra-Fast Booking Bot Started');
        console.log(`🎯 Target: ${CONFIG.TARGET_BUS_NUMBER}`);
        console.log(`⏱️  Max wait time: ${CONFIG.MAX_WAIT_TIME / 1000}s`);
        console.log('─'.repeat(50));
        
        // Global timeout
        globalTimer = setTimeout(() => {
            if (currentState !== STATE.COMPLETE) {
                setState(STATE.FAILED, 'Global timeout reached');
            }
        }, CONFIG.MAX_WAIT_TIME);
        
        // Start searching
        startSearching();
    }
    
    // Start the bot
    init();
    
    // Return control object for manual intervention if needed
    return {
        stop: cleanup,
        getState: () => currentState,
        retry: init
    };
}

// Execute the bot and store reference
const bot = ultraFastBookingBot();

// Optional: Add to window for manual control
window.bookingBot = bot;

console.log('💡 Tip: Use window.bookingBot.stop() to stop manually');
console.log('💡 Tip: Use window.bookingBot.retry() to restart');