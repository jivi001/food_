<?php
// Set error reporting for debugging
error_reporting(E_ALL);
ini_set('display_errors', 1);

// Function to validate input
function validateInput($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}

// Initialize response array
$response = [
    'success' => false,
    'message' => '',
    'orderId' => null,
    'receipt' => null,
    'items' => [],
    'totalCost' => 0
];

// Check if form was submitted
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Get selected items from form
    if(isset($_POST['item']) && is_array($_POST['item'])) {
        $selectedItems = $_POST['item'];
        
        // Menu items (must match with C program)
        $menuItems = [
            1 => ['name' => 'Biriyani', 'price' => 120],
            2 => ['name' => 'Full Meals', 'price' => 100],
            3 => ['name' => 'Mini Meal', 'price' => 80],
            4 => ['name' => 'Parotta', 'price' => 15],
            5 => ['name' => 'Soda', 'price' => 15],
            6 => ['name' => 'Ice Cream', 'price' => 50],
            7 => ['name' => 'Chicken Fry', 'price' => 200],
            8 => ['name' => 'Fish Fry', 'price' => 150],
            9 => ['name' => 'Paneer Butter Masala', 'price' => 120],
            10 => ['name' => 'Paneer Tikka', 'price' => 100]
        ];
        
        // Validate and prepare items for the C program
        $validItems = [];
        $orderTotal = 0;
        $itemDetailsForResponse = [];
        
        foreach($selectedItems as $itemId) {
            $itemId = (int)validateInput($itemId);
            
            if(isset($menuItems[$itemId])) {
                $validItems[] = $itemId;
                $orderTotal += $menuItems[$itemId]['price'];
                
                // Store item details for response
                $itemDetailsForResponse[] = [
                    'id' => $itemId,
                    'name' => $menuItems[$itemId]['name'],
                    'price' => $menuItems[$itemId]['price']
                ];
            }
        }
        
        if(count($validItems) > 0) {
            // Prepare command for the C program
            $itemString = implode(',', $validItems);
            
            // Create a temporary file to hold input for the C program
            $tempFile = tempnam(sys_get_temp_dir(), 'order_');
            file_put_contents($tempFile, $itemString . "\n0\n0\n"); // Add extra "0" to finish order, and "0" to exit
            
            // Execute the C program with the input file
            $command = './food_order_c < ' . $tempFile;
            $output = [];
            $returnVar = 0;
            
            exec($command, $output, $returnVar);
            
            // Clean up temporary file
            unlink($tempFile);
            
            // Parse the output to get receipt file path and order ID
            $receiptFile = null;
            $orderId = null;
            
            foreach($output as $line) {
                if(strpos($line, 'Receipt generated successfully:') !== false) {
                    $receiptFile = trim(str_replace('Receipt generated successfully:', '', $line));
                } else if(strpos($line, 'Order ID:') !== false) {
                    $orderId = (int)trim(str_replace('Order ID:', '', $line));
                }
            }
            
            // Check if order was placed successfully
            if($orderId !== null && $receiptFile !== null && file_exists($receiptFile)) {
                // Read receipt content
                $receiptContent = file_get_contents($receiptFile);
                
                $response['success'] = true;
                $response['message'] = 'Order placed successfully!';
                $response['orderId'] = $orderId;
                $response['receipt'] = $receiptContent;
                $response['items'] = $itemDetailsForResponse;
                $response['totalCost'] = $orderTotal;
            } else {
                $response['message'] = 'Failed to place order. C program execution failed.';
            }
        } else {
            $response['message'] = 'No valid items selected.';
        }
    } else {
        $response['message'] = 'No items selected.';
    }
}

// Option 1: Return JSON response for AJAX requests
if(isset($_SERVER['HTTP_X_REQUESTED_WITH']) && strtolower($_SERVER['HTTP_X_REQUESTED_WITH']) == 'xmlhttprequest') {
    header('Content-Type: application/json');
    echo json_encode($response);
    exit;
}

// Option 2: Redirect to a confirmation page with success/error message
if($response['success']) {
    // Redirect to confirmation page with order details
    header("Location: confirmation.php?orderId=" . $response['orderId']);
    exit;
} else {
    // Redirect back to menu with error message
    header("Location: index.html?error=" . urlencode($response['message']));
    exit;
}
?>