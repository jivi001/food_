document.addEventListener('DOMContentLoaded', function() {
    const checkboxes = document.querySelectorAll('input[type="checkbox"]');
    const selectedItemsDiv = document.getElementById('selected-items');
    const totalPriceSpan = document.getElementById('total-price');
    const orderForm = document.querySelector('form');
    
    // Menu items with their prices
    const menuItems = {
        '1': { name: 'Biriyani', price: 120 },
        '2': { name: 'Full Meals', price: 100 },
        '3': { name: 'Mini Meal', price: 80 },
        '4': { name: 'Parotta', price: 15 },
        '5': { name: 'Soda', price: 15 },
        '6': { name: 'Ice Cream', price: 50 },
        '7': { name: 'Chicken Fry', price: 200 },
        '8': { name: 'Fish Fry', price: 150 },
        '9': { name: 'Paneer Butter Masala', price: 120 },
        '10': { name: 'Paneer Tikka', price: 100 }
    };
    
    // Function to update order summary
    function updateOrderSummary() {
        let selectedItems = [];
        let totalPrice = 0;
        
        checkboxes.forEach(checkbox => {
            if (checkbox.checked) {
                const itemId = checkbox.value;
                const item = menuItems[itemId];
                selectedItems.push(item);
                totalPrice += item.price;
            }
        });
        
        // Update the selected items display
        if (selectedItems.length > 0) {
            selectedItemsDiv.innerHTML = selectedItems.map(item => 
                `<div class="selected-item">
                    <span>${item.name}</span>
                    <span class="item-price">₹${item.price}</span>
                </div>`
            ).join('');
            
            // Enable order button
            document.querySelector('.order-btn').disabled = false;
        } else {
            selectedItemsDiv.innerHTML = '<p>No items selected</p>';
            
            // Disable order button
            document.querySelector('.order-btn').disabled = true;
        }
        
        // Update the total price
        totalPriceSpan.textContent = `₹${totalPrice}`;
    }
    
    // Function to display error message
    function showError(message) {
        const errorDiv = document.createElement('div');
        errorDiv.className = 'error-message';
        errorDiv.style.backgroundColor = '#f8d7da';
        errorDiv.style.color = '#721c24';
        errorDiv.style.padding = '10px';
        errorDiv.style.marginBottom = '15px';
        errorDiv.style.borderRadius = '5px';
        errorDiv.style.textAlign = 'center';
        errorDiv.textContent = message;
        
        const container = document.querySelector('.menu');
        container.insertBefore(errorDiv, container.firstChild);
        
        // Remove after 5 seconds
        setTimeout(() => {
            errorDiv.remove();
        }, 5000);
    }
    
    // Add event listeners to checkboxes
    checkboxes.forEach(checkbox => {
        checkbox.addEventListener('change', updateOrderSummary);
    });
    
    // Check for URL parameters (for error messages)
    const urlParams = new URLSearchParams(window.location.search);
    if(urlParams.has('error')) {
        showError(urlParams.get('error'));
    }
    
    // Initially disable order button
    document.querySelector('.order-btn').disabled = true;
    
    // Optionally enhance with AJAX submission
    /* 
    orderForm.addEventListener('submit', function(e) {
        e.preventDefault();
        
        // Create FormData object
        const formData = new FormData(orderForm);
        
        // Send AJAX request
        fetch('process_order.php', {
            method: 'POST',
            body: formData,
            headers: {
                'X-Requested-With': 'XMLHttpRequest'
            }
        })
        .then(response => response.json())
        .then(data => {
            if(data.success) {
                // Redirect to confirmation page
                window.location.href = 'confirmation.php?orderId=' + data.orderId;
            } else {
                showError(data.message);
            }
        })
        .catch(error => {
            showError('An error occurred while processing your order. Please try again.');
            console.error('Error:', error);
        });
    });
    */
});