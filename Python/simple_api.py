from flask import Flask, jsonify, request

app = Flask(__name__)

# Sample data to simulate a database
data_store = {
    1: {"name": "Item 1", "description": "This is the first item"},
    2: {"name": "Item 2", "description": "This is the second item"}
}

# Home route
@app.route("/", methods=["GET"])
def home():
    """
    Returns a JSON response with a welcome message and an HTTP status code.
    
    Returns:
        tuple: A tuple containing a JSON response with a welcome message and an HTTP status code 200.
    """
    return jsonify({"message": "Welcome to the Simple API!"}), 200

# Get all items
@app.route("/items", methods=["GET"])
def get_items():
    """Retrieves items from the data store and returns them as a JSON response.
    
    Returns:
        tuple: A tuple containing the JSON representation of the items in the data store and an HTTP status code 200.
    """
    return jsonify(data_store), 200

# Get a single item by ID
@app.route("/items/<int:item_id>", methods=["GET"])
def get_item(item_id):
    """
    Retrieves an item from the data store by its ID and returns it in JSON format.
    
    Args:
        item_id (str): The unique identifier of the item to retrieve.
    
    Returns:
        (tuple): A tuple containing:
            - A JSON response with the item data (dict) if found, or an error message (dict) if not found.
            - HTTP status code (int), which is 200 if the item is found, or 404 if not found.
    """
    item = data_store.get(item_id)
    if item:
        return jsonify(item), 200
    else:
        return jsonify({"error": "Item not found"}), 404

# Add a new item
@app.route("/items", methods=["POST"])
def add_item():
    """Adds a new item to the data store from the JSON payload received in the request.
    
    This function checks if the required fields 'name' and 'description' are present in the request body. 
    If any of these fields are missing, it returns an error response. If all required fields are present, 
    it assigns a new unique ID to the item, adds it to the data store, and returns a success response with the new item ID.
    
    Returns:
        tuple: A response object containing a JSON message indicating success or failure,
               and an HTTP status code (400 if data is invalid, 201 if item is successfully added).
    """
    new_item = request.json
    if not new_item.get("name") or not new_item.get("description"):
        return jsonify({"error": "Invalid data"}), 400

    new_id = max(data_store.keys()) + 1 if data_store else 1
    data_store[new_id] = {"name": new_item["name"], "description": new_item["description"]}
    return jsonify({"message": "Item added", "item_id": new_id}), 201

# Delete an item by ID
@app.route("/items/<int:item_id>", methods=["DELETE"])
def delete_item(item_id):
    """Deletes an item from the data store.
    
    Args:
        item_id (str): The unique identifier of the item to be deleted.
    
    Returns:
        tuple: A tuple containing a JSON response and an HTTP status code. 
               Returns a success message and code 200 if the item was deleted,
               or an error message and code 404 if the item was not found.
    """
    
    if item_id in data_store:
        del data_store[item_id]
        return jsonify({"message": "Item deleted"}), 200
    else:
        return jsonify({"error": "Item not found"}), 404

# Run the app
if __name__ == "__main__":
    app.run(debug=True)