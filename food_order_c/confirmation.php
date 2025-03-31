<?php
// Check if order ID is provided
if(!isset($_GET['orderId'])) {
    header("Location: index.html");
    exit;
}

$orderId = (int)$_GET['orderId'];
$receiptFile = "receipt_order_" . $orderId . ".txt";
$receiptContent = "";

// Check if receipt file exists
if(file_exists($receiptFile)) {
    $receiptContent = file_get_contents($receiptFile);
} else {
    $receiptContent = "Receipt not found. Please contact support.";
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Order Confirmation - Thalapakatti Biriyani</title>
    <link rel="stylesheet" href="styles.css">
    <style>
        .receipt-container {
            background-color: white;
            padding: 20px;
            border-radius: 12px;
            box-shadow: 0 4px 15px rgba(0, 0, 0, 0.1);
            max-width: 600px;
            margin: 20px auto;
            font-family: monospace;
            white-space: pre-wrap;
            line-height: 1.4;
        }
        
        .confirmation-message {
            text-align: center;
            margin-bottom: 20px;
            color: #d35400;
        }
        
        .back-btn {
            background-color: #e67e22;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            display: inline-block;
            margin-top: 20px;
            text-decoration: none;
            font-weight: bold;
        }
        
        .back-btn:hover {
            background-color: #d35400;
        }
        
        .buttons {
            text-align: center;
            margin-top: 20px;
        }
        
        .print-btn {
            background-color: #3498db;
            color: white;
            padding: 10px 20px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            display: inline-block;
            margin-right: 10px;
            text-decoration: none;
            font-weight: bold;
        }
        
        .print-btn:hover {
            background-color: #2980b9;
        }
    </style>
</head>
<body>
    <header>
        <h1>THALAPAKATTI BIRIYANI</h1>
        <p class="tagline">Authentic South Indian Cuisine</p>
    </header>

    <div class="container">
        <h2 class="confirmation-message">Thank you for your order!</h2>
        
        <div class="receipt-container">
            <?php echo htmlspecialchars($receiptContent); ?>
        </div>
        
        <div class="buttons">
            <button class="print-btn" onclick="window.print();">Print Receipt</button>
            <a href="index.html" class="back-btn">Place Another Order</a>
        </div>
    </div>

    <footer>
        <p>&copy; 2025 Thalapakatti Biriyani. All rights reserved.</p>
    </footer>
</body>
</html>