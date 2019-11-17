/**

MIT License

Copyright (c) 2019 mpomaranski at gmail

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#pragma once

#include <string>

namespace cleric {
    namespace data {
    /*

A thing Resource provides a Thing Description for a device. A Thing Resource is considered the root resource of a Thing.

@see https://iot.mozilla.org/wot/
*/
    class WebThing
    {
    public:
        /*
	  A property object describes an attribute of a Thing and is indexed by a property id. 

	  A property description includes:
		- primitive type (one of null, boolean, object, array, number, integer or string as per [json-schema])
		- semantic @type (a string identifying a type from the linked @context)
		- unit ([SI] unit)
		- title (A string providing a human friendly name)
		- description (A string providing a human friendly description)
		- links array (An array of Link objects linking to one or more representations of a Property resource, each with an implied default rel=property.)
		- enum (an enumeration of possible values for the property)
		- readOnly (A boolean indicating whether or not the property is read-only, defaulting to false)
		- A minimum and maximum (numeric values)
		- multipleOf (A number indicating what the value must be a multiple of)

		Example:
			"level" : {
				"title": "Level",
				"description": "The level of light from 0-100",
				"@type": "LevelProperty",
				"type": "integer",
				"unit": "percent",
				"minimum": 0,
				"maximum": 100,
				"readOnly": false,
				"links": [{"href": "/things/lamp/properties/level"}]
			}
	   */
        using PropertyValue = ::boost::variant<bool, double, ::std::string>;
        class Property
        {
        public:
            SMART_ENUM(Type, BOOLEAN, NUMBER, STRING);
            string getTitle() const { return title; }
            Type getPropertyType() const { return type; };
            string getReadableDescription() const { return humanDescription; };
            string getUnit() const { return unit; };
            double getMinimum() const { return minimum; }
            double getMaximum() const { return maximum; }
            bool isReadOnly() const { return readOnly; }
            string getLink() const { return link; }

            string getJson() const;

            // sets value, returns the new value (if differs, there is an error with setting)
            PropertyValue set(const PropertyValue &val);
            PropertyValue get();

            // in a json format as defined above
            string getDescription() const;

        private:
            Type type;
            const string title;
            const string humanDescription;
            const string unit;
            const double minimum;
            const double maximum;
            const bool readOnly;
            const string link;

            const string cachedDescription;
        };

        Property &getProperty(const string &title);

        string getDescription() const;

        string id;
        string title;
        string description;

        ::std::string actions;
        ::std::string events;

        string requestAction(const string &action);

    private:
        int pendingActions;
        int completedActions;

        int events;
    };

    class Action
    {
    public:
        string id;
        string href;
        SMART_ENUM(ActionStatus, pending, completed);
        int timestampRequested;
        int timestampCompleted;
    };
    }
}