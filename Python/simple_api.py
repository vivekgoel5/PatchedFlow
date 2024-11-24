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
    return jsonify({"message": "Welcome to the Simple API!"}), 200

# Get all items
@app.route("/items", methods=["GET"])
def get_items():
    return jsonify(data_store), 200

# Get a single item by ID
@app.route("/items/<int:item_id>", methods=["GET"])
def get_item(item_id):
    item = data_store.get(item_id)
    if item:
        return jsonify(item), 200
    else:
        return jsonify({"error": "Item not found"}), 404

# Add a new item
@app.route("/items", methods=["POST"])
def add_item():
    new_item = request.json
    if not new_item.get("name") or not new_item.get("description"):
        return jsonify({"error": "Invalid data"}), 400

    new_id = max(data_store.keys()) + 1 if data_store else 1
    data_store[new_id] = {"name": new_item["name"], "description": new_item["description"]}
    return jsonify({"message": "Item added", "item_id": new_id}), 201

# Delete an item by ID
@app.route("/items/<int:item_id>", methods=["DELETE"])
def delete_item(item_id):
    if item_id in data_store:
        del data_store[item_id]
        return jsonify({"message": "Item deleted"}), 200
    else:
        return jsonify({"error": "Item not found"}), 404

# Run the app
if __name__ == "__main__":
    app.run(debug=True)